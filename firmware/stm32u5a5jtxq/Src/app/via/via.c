#include "via.h"
#include "eeconfig.h"

static const char *command_id_str[] =
{
  [id_get_protocol_version]                 = "id_get_protocol_version",
  [id_get_keyboard_value]                   = "id_get_keyboard_value",
  [id_set_keyboard_value]                   = "id_set_keyboard_value",
  [id_dynamic_keymap_get_keycode]           = "id_dynamic_keymap_get_keycode",
  [id_dynamic_keymap_set_keycode]           = "id_dynamic_keymap_set_keycode",
  [id_dynamic_keymap_reset]                 = "id_dynamic_keymap_reset",
  [id_custom_set_value]                     = "id_custom_set_value",
  [id_custom_get_value]                     = "id_custom_get_value",
  [id_custom_save]                          = "id_custom_save",
  [id_eeprom_reset]                         = "id_eeprom_reset",
  [id_bootloader_jump]                      = "id_bootloader_jump",
  [id_dynamic_keymap_macro_get_count]       = "id_dynamic_keymap_macro_get_count",
  [id_dynamic_keymap_macro_get_buffer_size] = "id_dynamic_keymap_macro_get_buffer_size",
  [id_dynamic_keymap_macro_get_buffer]      = "id_dynamic_keymap_macro_get_buffer",
  [id_dynamic_keymap_macro_set_buffer]      = "id_dynamic_keymap_macro_set_buffer",
  [id_dynamic_keymap_macro_reset]           = "id_dynamic_keymap_macro_reset",
  [id_dynamic_keymap_get_layer_count]       = "id_dynamic_keymap_get_layer_count",
  [id_dynamic_keymap_get_buffer]            = "id_dynamic_keymap_get_buffer",
  [id_dynamic_keymap_set_buffer]            = "id_dynamic_keymap_set_buffer",
  [id_dynamic_keymap_get_encoder]           = "id_dynamic_keymap_get_encoder",
  [id_dynamic_keymap_set_encoder]           = "id_dynamic_keymap_set_encoder",
  [id_vial_prefix]                          = "id_vial_prefix",
  [id_unhandled]                            = "id_unhandled",
};

#define MATRIX_ROWS                         5
#define MATRIX_COLS                         15

#define DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR    0
#define DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE    1024
#define DYNAMIC_KEYMAP_LAYER_COUNT          4
#define DYNAMIC_KEYMAP_MACRO_COUNT          16

#define DYNAMIC_KEYMAP_EEPROM_START         (EECONFIG_SIZE)
#define DYNAMIC_KEYMAP_EEPROM_ADDR          DYNAMIC_KEYMAP_EEPROM_START


#if (MATRIX_COLS <= 8)
typedef uint8_t matrix_row_t;
#elif (MATRIX_COLS <= 16)
typedef uint16_t matrix_row_t;
#elif (MATRIX_COLS <= 32)
typedef uint32_t matrix_row_t;
#else
#    error "MATRIX_COLS: invalid value"
#endif

#define VIA_EEPROM_MAGIC_ADDR             (EECONFIG_SIZE)
#define VIA_EEPROM_LAYOUT_OPTIONS_ADDR    (VIA_EEPROM_MAGIC_ADDR + 3)
#define VIA_EEPROM_LAYOUT_OPTIONS_SIZE    1
#define VIA_EEPROM_LAYOUT_OPTIONS_DEFAULT 0x00000000
// The end of the EEPROM memory used by VIA
// By default, dynamic keymaps will start at this if there is no
// custom config
#define VIA_EEPROM_CUSTOM_CONFIG_ADDR     (VIA_EEPROM_LAYOUT_OPTIONS_ADDR + VIA_EEPROM_LAYOUT_OPTIONS_SIZE)
#define VIA_EEPROM_CUSTOM_CONFIG_SIZE     0
#define VIA_EEPROM_CONFIG_END             (VIA_EEPROM_CUSTOM_CONFIG_ADDR + VIA_EEPROM_CUSTOM_CONFIG_SIZE)


static uint8_t via_eeprom_buf[DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE];
static matrix_row_t matrix[MATRIX_ROWS] = {0, };


static matrix_row_t matrix_get_row(uint8_t row);
static uint32_t     via_get_layout_options(void);
static void         raw_hid_receive_kb(uint8_t *data, uint8_t length);
static void         viaHidCmdPrint(uint8_t *data, uint8_t length, bool is_resp);
static bool         viaHidCustomCmd(uint8_t *data, uint8_t length);

static void         dynamic_keymap_macro_get_buffer(uint16_t offset, uint16_t size, uint8_t *data);
static void         dynamic_keymap_get_buffer(uint16_t offset, uint16_t size, uint8_t *data);
static void         dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t column, uint16_t keycode);
static void         dynamic_keymap_macro_reset(void);
static void         dynamic_keymap_set_buffer(uint16_t offset, uint16_t size, uint8_t *data);
void                dynamic_keymap_macro_set_buffer(uint16_t offset, uint16_t size, uint8_t *data);



void viaHidReceive(uint8_t *data, uint8_t length)
{
  uint8_t *command_id   = &(data[0]);
  uint8_t *command_data = &(data[1]);  
  bool is_resp = true;


  switch (*command_id)
  {
    case id_get_protocol_version:
      {
        command_data[0] = VIA_PROTOCOL_VERSION >> 8;
        command_data[1] = VIA_PROTOCOL_VERSION & 0xFF;
        break;
      }

    case id_dynamic_keymap_macro_get_buffer_size:
      {
        uint16_t size   = DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE;
        command_data[0] = size >> 8;
        command_data[1] = size & 0xFF;
        break;
      }

    case id_dynamic_keymap_macro_get_count:
      {
        command_data[0] = DYNAMIC_KEYMAP_MACRO_COUNT;
        break;
      }

    case id_dynamic_keymap_macro_get_buffer:
      {
        uint16_t offset = (command_data[0] << 8) | command_data[1];
        uint16_t size   = command_data[2]; // size <= 28
        if (size <= 28)
          dynamic_keymap_macro_get_buffer(offset, size, &command_data[3]);
        break;
      }

    case id_dynamic_keymap_get_layer_count:
      {
        command_data[0] = DYNAMIC_KEYMAP_LAYER_COUNT;
        break;
      }

    case id_get_keyboard_value:
      {
        switch (command_data[0])
        {
          case id_uptime:
            {
              uint32_t value  = millis();
              command_data[1] = (value >> 24) & 0xFF;
              command_data[2] = (value >> 16) & 0xFF;
              command_data[3] = (value >> 8) & 0xFF;
              command_data[4] = value & 0xFF;
              break;
            }
          case id_layout_options:
            {
              uint32_t value  = via_get_layout_options();
              command_data[1] = (value >> 24) & 0xFF;
              command_data[2] = (value >> 16) & 0xFF;
              command_data[3] = (value >> 8) & 0xFF;
              command_data[4] = value & 0xFF;
              break;
            }
          case id_switch_matrix_state:
            {

#if ((MATRIX_COLS / 8 + 1) * MATRIX_ROWS <= 28)
              uint8_t i = 1;
              for (uint8_t row = 0; row < MATRIX_ROWS; row++)
              {
                matrix_row_t value = matrix_get_row(row);
#if (MATRIX_COLS > 24)
                command_data[i++] = (value >> 24) & 0xFF;
#endif
#if (MATRIX_COLS > 16)
                command_data[i++] = (value >> 16) & 0xFF;
#endif
#if (MATRIX_COLS > 8)
                command_data[i++] = (value >> 8) & 0xFF;
#endif
                command_data[i++] = value & 0xFF;
              }
#endif
              break;
            }
          default:
            {
              raw_hid_receive_kb(data, length);
              break;
            }
        }
        break;
      }

    case id_dynamic_keymap_get_buffer:
      {
        uint16_t offset = (command_data[0] << 8) | command_data[1];
        uint16_t size   = command_data[2]; // size <= 28
        if (size <= 28)
          dynamic_keymap_get_buffer(offset, size, &command_data[3]);
        break;
      }

    case id_dynamic_keymap_set_keycode:
      {
        dynamic_keymap_set_keycode(command_data[0], command_data[1], command_data[2], (command_data[3] << 8) | command_data[4]);
        break;
      }

    case id_dynamic_keymap_macro_reset:
      {
        dynamic_keymap_macro_reset();
        break;
      }

    case id_dynamic_keymap_set_buffer:
      {
        uint16_t offset = (command_data[0] << 8) | command_data[1];
        uint16_t size   = command_data[2]; // size <= 28
        if (size <= 28)
          dynamic_keymap_set_buffer(offset, size, &command_data[3]);
        break;
      }

    case id_dynamic_keymap_macro_set_buffer:
      {
#ifdef VIAL_ENABLE
        /* Until keyboard is unlocked, don't allow changing macros */
        if (!vial_unlocked)
          goto skip;
#endif
        uint16_t offset = (command_data[0] << 8) | command_data[1];
        uint16_t size   = command_data[2]; // size <= 28
        if (size <= 28)
          dynamic_keymap_macro_set_buffer(offset, size, &command_data[3]);
        break;
      }

    case id_custom_set_value:
    case id_custom_get_value:
    case id_custom_save:
      {
        is_resp = viaHidCustomCmd(data, length);
        break;
      }

    default:
      is_resp = false;
      break;
  }

  viaHidCmdPrint(data, length, is_resp);

  if (!is_resp)
  {
    raw_hid_receive_kb(data, length);
  }
}

bool viaHidCustomCmd(uint8_t *data, uint8_t length)
{
  uint8_t *command_id = &(data[0]);
  uint8_t *channel_id = &(data[1]);
  uint8_t *value_id   = &(data[2]);
  uint8_t *value_data = &(data[3]);


  if (*channel_id == 3)
  {
    static uint8_t value = 0;

    switch (*command_id)
    {
      case id_custom_set_value:
        {
          value = value_data[0];
          break;
        }
      case id_custom_get_value:
        {
          value_data[0] = value;
          break;
        }
      case id_custom_save:
        {
          break;
        }
      default:
        {
          *command_id = id_unhandled;
          break;
        }
    }
    return true;
  }
  // raw_hid_receive_kb(data, length); 

  return false;
}

void viaHidCmdPrint(uint8_t *data, uint8_t length, bool is_resp)
{
  uint8_t *command_id   = &(data[0]);
  uint8_t *command_data = &(data[1]);  
  uint8_t data_len = length - 1;


  logPrintf("[%s] id : 0x%02X, 0x%02X, len %d,  %s",
            is_resp == true ? "OK" : "  ",
            *command_id,
            command_data[0],
            length,
            command_id_str[*command_id]);

#ifdef _USE_VIA_SHOW
  is_resp = false;
#else
  is_resp = true;
#endif

  if (is_resp == true)
  {
    logPrintf("\n\r");
    return;
  }

  for (int i=0; i<data_len; i++)
  {
    if (i%8 == 0)
      logPrintf("\n\r     ");
    logPrintf("0x%02X ", command_data[i]);
  }
  logPrintf("\n\r");  
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length)
{
  uint8_t *command_id = &(data[0]);
  *command_id         = id_unhandled;
}

uint8_t eeprom_read_byte(const uint8_t *addr)
{
  return via_eeprom_buf[(uint32_t)addr];
}

void eeprom_write_byte(uint8_t *addr, uint8_t data)
{
  via_eeprom_buf[(uint32_t)addr] = data;
}

void eeprom_update_byte(uint8_t *addr, uint8_t value)
{
  eeprom_write_byte(addr, value);
}

void dynamic_keymap_macro_get_buffer(uint16_t offset, uint16_t size, uint8_t *data)
{
  void    *source = (void *)(DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + offset);
  uint8_t *target = data;
  for (uint16_t i = 0; i < size; i++)
  {
    if (offset + i < DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE)
    {
      *target = eeprom_read_byte(source);
    }
    else
    {
      *target = 0x00;
    }
    source++;
    target++;
  }
}

void dynamic_keymap_get_buffer(uint16_t offset, uint16_t size, uint8_t *data)
{
  uint16_t dynamic_keymap_eeprom_size = DYNAMIC_KEYMAP_LAYER_COUNT * MATRIX_ROWS * MATRIX_COLS * 2;
  void    *source                     = (void *)(DYNAMIC_KEYMAP_EEPROM_ADDR + offset);
  uint8_t *target                     = data;
  for (uint16_t i = 0; i < size; i++)
  {
    if (offset + i < dynamic_keymap_eeprom_size)
    {
      *target = eeprom_read_byte(source);
    }
    else
    {
      *target = 0x00;
    }
    source++;
    target++;
  }
}

matrix_row_t matrix_get_row(uint8_t row)
{
  // Matrix mask lets you disable switches in the returned matrix data. For example, if you have a
  // switch blocker installed and the switch is always pressed.
#ifdef MATRIX_MASKED
  return matrix[row] & matrix_mask[row];
#else
  return matrix[row];
#endif
}

// This is generalized so the layout options EEPROM usage can be
// variable, between 1 and 4 bytes.
uint32_t via_get_layout_options(void)
{
  uint32_t value = 0;
  // Start at the most significant byte
  void *source = (void *)(VIA_EEPROM_LAYOUT_OPTIONS_ADDR);
  for (uint8_t i = 0; i < VIA_EEPROM_LAYOUT_OPTIONS_SIZE; i++)
  {
    value  = value << 8;
    value |= eeprom_read_byte(source);
    source++;
  }
  return value;
}

void *dynamic_keymap_key_to_eeprom_address(uint8_t layer, uint8_t row, uint8_t column)
{
  // TODO: optimize this with some left shifts
  return ((void *)DYNAMIC_KEYMAP_EEPROM_ADDR) + (layer * MATRIX_ROWS * MATRIX_COLS * 2) + (row * MATRIX_COLS * 2) + (column * 2);
}

void dynamic_keymap_set_keycode(uint8_t layer, uint8_t row, uint8_t column, uint16_t keycode)
{
  if (layer >= DYNAMIC_KEYMAP_LAYER_COUNT || row >= MATRIX_ROWS || column >= MATRIX_COLS) return;
  void *address = dynamic_keymap_key_to_eeprom_address(layer, row, column);
  // Big endian, so we can read/write EEPROM directly from host if we want
  eeprom_update_byte(address, (uint8_t)(keycode >> 8));
  eeprom_update_byte(address + 1, (uint8_t)(keycode & 0xFF));
}

void dynamic_keymap_macro_reset(void)
{
  void *p   = (void *)(DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR);
  void *end = (void *)(DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE);
  while (p != end)
  {
    eeprom_update_byte(p, 0);
    ++p;
  }
}

void dynamic_keymap_set_buffer(uint16_t offset, uint16_t size, uint8_t *data)
{
  uint16_t dynamic_keymap_eeprom_size = DYNAMIC_KEYMAP_LAYER_COUNT * MATRIX_ROWS * MATRIX_COLS * 2;
  void    *target                     = (void *)(DYNAMIC_KEYMAP_EEPROM_ADDR + offset);
  uint8_t *source                     = data;

#ifdef VIAL_ENABLE
  /* ensure the writes are bounded */
  if (offset >= dynamic_keymap_eeprom_size || dynamic_keymap_eeprom_size - offset < size)
    return;

#ifndef VIAL_INSECURE
  /* Check whether it is trying to send a QK_BOOT keycode; only allow setting these if unlocked */
  if (!vial_unlocked)
  {
    /* how much of the input array we'll have to check in the loop */
    uint16_t chk_offset = 0;
    uint16_t chk_sz     = size;

    /* initial byte misaligned -- this means the first keycode will be a combination of existing and new data */
    if (offset % 2 != 0)
    {
      uint16_t kc = (eeprom_read_byte((uint8_t *)target - 1) << 8) | data[0];
      if (kc == QK_BOOT)
        data[0] = 0xFF;

      /* no longer have to check the first byte */
      chk_offset += 1;
    }

    /* final byte misaligned -- this means the last keycode will be a combination of new and existing data */
    if ((offset + size) % 2 != 0)
    {
      uint16_t kc = (data[size - 1] << 8) | eeprom_read_byte((uint8_t *)target + size);
      if (kc == QK_BOOT)
        data[size - 1] = 0xFF;

      /* no longer have to check the last byte */
      chk_sz -= 1;
    }

    /* check the entire array, replace any instances of QK_BOOT with invalid keycode 0xFFFF */
    for (uint16_t i = chk_offset; i < chk_sz; i += 2)
    {
      uint16_t kc = (data[i] << 8) | data[i + 1];
      if (kc == QK_BOOT)
      {
        data[i]     = 0xFF;
        data[i + 1] = 0xFF;
      }
    }
  }
#endif
#endif

  for (uint16_t i = 0; i < size; i++)
  {
    if (offset + i < dynamic_keymap_eeprom_size)
    {
      eeprom_update_byte(target, *source);
    }
    source++;
    target++;
  }
}

void dynamic_keymap_macro_set_buffer(uint16_t offset, uint16_t size, uint8_t *data)
{
  void    *target = (void *)(DYNAMIC_KEYMAP_MACRO_EEPROM_ADDR + offset);
  uint8_t *source = data;
  for (uint16_t i = 0; i < size; i++)
  {
    if (offset + i < DYNAMIC_KEYMAP_MACRO_EEPROM_SIZE)
    {
      eeprom_update_byte(target, *source);
    }
    source++;
    target++;
  }
}