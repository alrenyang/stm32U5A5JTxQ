#ifndef KEYSCAN_H_
#define KEYSCAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"



typedef struct
{
  uint8_t modifier;
  uint8_t reserved;
  uint8_t keycode[HW_KEYSCAN_PRESS_MAX];
} keyscan_keycode_t;


bool keyscanInit(void);
void keyscanUpdate(void);
bool keyscanGetKeyCode(keyscan_keycode_t *p_keycode);

bool keyscanGetChangedCode(keyscan_keycode_t *p_keycode);
bool keyscanGetPressedCode(keyscan_keycode_t *p_keycode);

#ifdef __cplusplus
}
#endif

#endif