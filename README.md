Compilar no Nano

```shell
arduino-cli compile \
  --fqbn arduino:avr:nano:cpu=atmega328 \
  --config-file arduino-cli.yaml \
  --verbose \
  main

```



Upload no Nano
```shell
arduino-cli upload \
  --fqbn arduino:avr:nano:cpu=atmega328old \
  --port /dev/cu.usbserial-1120 \
  --config-file arduino-cli.yaml \
  --verbose \
  main

```


Monitorar Porta Serial

```
arduino-cli monitor -p /dev/cu.usbserial-1120 baudrate=9600
```

```
lsof /dev/cu.usbserial-1120
kill -9 <PID>
```
