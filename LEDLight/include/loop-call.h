#pragma once

#define CALL_LOOP(TIME, FUNC) {static unsigned long fn_##FUNC_##TIME_counter;\
  if (millis() -  fn_##FUNC_##TIME_counter > (TIME)) {\
    fn_##FUNC_##TIME_counter = millis();\
    FUNC();}\
  }
  