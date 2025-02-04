#ifndef VIA_H_
#define VIA_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"


// This is changed only when the command IDs change,
// so VIA Configurator can detect compatible firmware.
#define VIA_PROTOCOL_VERSION 0x000C

// This is a version number for the firmware for the keyboard.
// It can be used to ensure the VIA keyboard definition and the firmware
// have the same version, especially if there are changes to custom values.
// Define this in config.h to override and bump this number.
// This is *not* required if the keyboard is only using basic functionality
// and not using custom values for lighting, rotary encoders, etc.
#ifndef VIA_FIRMWARE_VERSION
#    define VIA_FIRMWARE_VERSION 0x00000000
#endif

enum via_command_id {
    id_get_protocol_version                 = 0x01, // always 0x01
    id_get_keyboard_value                   = 0x02,
    id_set_keyboard_value                   = 0x03,
    id_dynamic_keymap_get_keycode           = 0x04,
    id_dynamic_keymap_set_keycode           = 0x05,
    id_dynamic_keymap_reset                 = 0x06,
    id_custom_set_value                     = 0x07,
    id_custom_get_value                     = 0x08,
    id_custom_save                          = 0x09,
    id_eeprom_reset                         = 0x0A,
    id_bootloader_jump                      = 0x0B,
    id_dynamic_keymap_macro_get_count       = 0x0C,
    id_dynamic_keymap_macro_get_buffer_size = 0x0D,
    id_dynamic_keymap_macro_get_buffer      = 0x0E,
    id_dynamic_keymap_macro_set_buffer      = 0x0F,
    id_dynamic_keymap_macro_reset           = 0x10,
    id_dynamic_keymap_get_layer_count       = 0x11,
    id_dynamic_keymap_get_buffer            = 0x12,
    id_dynamic_keymap_set_buffer            = 0x13,
    id_dynamic_keymap_get_encoder           = 0x14,
    id_dynamic_keymap_set_encoder           = 0x15,    
    id_vial_prefix                          = 0xFE,
    id_unhandled                            = 0xFF,
};

enum via_keyboard_value_id {
    id_uptime              = 0x01,
    id_layout_options      = 0x02,
    id_switch_matrix_state = 0x03,
    id_firmware_version    = 0x04,
    id_device_indication   = 0x05,
};

enum via_channel_id {
    id_custom_channel         = 0,
    id_qmk_backlight_channel  = 1,
    id_qmk_rgblight_channel   = 2,
    id_qmk_rgb_matrix_channel = 3,
    id_qmk_audio_channel      = 4,
    id_qmk_led_matrix_channel = 5,
};

enum via_qmk_backlight_value {
    id_qmk_backlight_brightness = 1,
    id_qmk_backlight_effect     = 2,
};

enum via_qmk_rgblight_value {
    id_qmk_rgblight_brightness   = 1,
    id_qmk_rgblight_effect       = 2,
    id_qmk_rgblight_effect_speed = 3,
    id_qmk_rgblight_color        = 4,
};

enum via_qmk_rgb_matrix_value {
    id_qmk_rgb_matrix_brightness   = 1,
    id_qmk_rgb_matrix_effect       = 2,
    id_qmk_rgb_matrix_effect_speed = 3,
    id_qmk_rgb_matrix_color        = 4,
};

enum via_qmk_led_matrix_value {
    id_qmk_led_matrix_brightness   = 1,
    id_qmk_led_matrix_effect       = 2,
    id_qmk_led_matrix_effect_speed = 3,
};

enum via_qmk_audio_value {
    id_qmk_audio_enable        = 1,
    id_qmk_audio_clicky_enable = 2,
};



void viaHidReceive(uint8_t *data, uint8_t length);



#ifdef __cplusplus
}
#endif

#endif 