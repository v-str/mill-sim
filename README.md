# vmill-server

Сервер фрезерного станка. Работает в паре с андройд-приложением vmill.

## Требования

- Linux
- GCC
- Boost ≥ 1.91.0 (asio)
- OpenSSL

## Сборка

```bash
cmake -B build
cmake --build build
```

## Зависимости

Используются системные библиотеки Boost и OpenSSL.
