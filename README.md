# Lab 01 — GPIO: LED Blink & Button

## Mục tiêu

- Cấu hình GPIO output để điều khiển LED
- Cấu hình GPIO input để đọc trạng thái nút nhấn
- Xuất kết quả qua UART để CI kiểm tra

## Phần cứng (Renode simulation)

| Chức năng | Pin     | Ghi chú               |
|-----------|---------|-----------------------|
| LED xanh  | PD12    | STM32F4-Discovery     |
| LED cam   | PD13    | STM32F4-Discovery     |
| LED đỏ    | PD14    | STM32F4-Discovery     |
| LED xanh lá | PD15  | STM32F4-Discovery     |
| UART TX   | PA2     | USART2 (debug output) |

## Yêu cầu

Hoàn thành các hàm trong `src/main.c`:

1. `gpio_init()` — Cấu hình PD12–PD15 là output, PA2 là AF (USART2)
2. `led_set(pin, state)` — Bật/tắt LED theo pin
3. `led_toggle(pin)` — Toggle LED
4. Chạy test sequence và in kết quả qua UART

## Test Cases

| Test | Mô tả                          | Expected Output         |
|------|--------------------------------|-------------------------|
| TC1  | Bật LED PD12, đọc lại ODR      | `TC1:PASS`              |
| TC2  | Toggle LED PD13 2 lần          | `TC2:PASS`              |
| TC3  | Bật tất cả LED, kiểm tra ODR   | `TC3:PASS`              |
| TC4  | Tắt tất cả LED, kiểm tra ODR   | `TC4:PASS`              |

## Chạy local (nếu có Renode)

```bash
make
renode --disable-xwt sim/run.resc
```

## Nộp bài

```bash
git checkout -b lab01/<tên-sinh-viên>
# Sửa src/main.c
git add src/main.c
git commit -m "lab01: implement GPIO control"
git push origin lab01/<tên-sinh-viên>
```
