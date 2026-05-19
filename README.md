# v-arm-server

Сервер-эмулятор роборуки. Работает в паре с v-arm.

## Работа с Git Submodules

`third_party/asio` — git submodule из https://github.com/boostorg/asio.git.

### Клонирование проекта вместе с submodules

```bash
git clone --recursive <url-репозитория>
```

Если проект уже склонирован без `--recursive`, подтянуть submodules:

```bash
git submodule update --init --recursive
```

### Обновление submodule до последней версии

```bash
cd third_party/asio
git checkout master
git pull
cd ../..
git add third_party/asio
git commit -m "update: asio submodule"
```

### Восстановление submodule при удалении

Если `third_party/asio` был случайно удалён:

```bash
git submodule update --init --recursive
```