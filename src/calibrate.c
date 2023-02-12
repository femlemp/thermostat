// Значение температуры, возвращаемое если сумма результатов АЦП больше первого значения таблицы
#define TEMPERATURE_UNDER 0
// Значение температуры, возвращаемое если сумма результатов АЦП меньше последнего значения таблицы
#define TEMPERATURE_OVER 0
// Значение температуры соответствующее первому значению таблицы
#define TEMPERATURE_TABLE_START 0
// Шаг таблицы 
#define TEMPERATURE_TABLE_STEP 50


// Метод доступа к элементу таблицы, должна соответствовать 16bit
#define TEMPERATURE_TABLE_READ(i) pgm_read_word(&termo_table[i])

/* Таблица суммарного значения АЦП в зависимости от температуры. От большего значения к меньшему
   Для построения таблицы использованы следующие парамертры:
*/
const uint16_t termo_table[] PROGMEM = {
  0 
};

// Функция вычисляет значение температуры в десятых долях градусов Цельсия
// в зависимости от суммарного значения АЦП.
int16_t calc_temperature(uint16_t adcsum) {
  uint8_t l = 0;
  uint8_t r = (sizeof(termo_table) / sizeof(termo_table[0])) - 1;
  uint16_t thigh = TEMPERATURE_TABLE_READ(r);
  
  // Проверка выхода за пределы и граничных значений
  if (adcsum <= thigh) {
    #ifdef TEMPERATURE_UNDER
      if (adcsum < thigh) 
        return TEMPERATURE_UNDER;
    #endif
    return TEMPERATURE_TABLE_STEP * r + TEMPERATURE_TABLE_START;
  }
  uint16_t tlow = TEMPERATURE_TABLE_READ(0);
  if (adcsum >= tlow) {
    #ifdef TEMPERATURE_OVER
      if (adcsum > tlow)
        return TEMPERATURE_OVER;
    #endif
    return TEMPERATURE_TABLE_START;
  }

  // Двоичный поиск по таблице
  while ((r - l) > 1) {
    uint8_t m = (l + r) >> 1;
    uint16_t mid = TEMPERATURE_TABLE_READ(m);
    if (adcsum > mid) {
      r = m;
    } else {
      l = m;
    }
  }
  uint16_t vl = TEMPERATURE_TABLE_READ(l);
  if (adcsum >= vl) {
    return l * TEMPERATURE_TABLE_STEP + TEMPERATURE_TABLE_START;
  }
  uint16_t vr = TEMPERATURE_TABLE_READ(r);
  uint16_t vd = vl - vr;
  int16_t res = TEMPERATURE_TABLE_START + r * TEMPERATURE_TABLE_STEP; 
  if (vd) {
    // Линейная интерполяция
    res -= ((TEMPERATURE_TABLE_STEP * (int32_t)(adcsum - vr) + (vd >> 1)) / vd);
  }
  return res;
}

