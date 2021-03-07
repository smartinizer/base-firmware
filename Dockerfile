FROM python:3.9 AS build
WORKDIR /usr/src/app
COPY /. .
RUN pip install -U platformio
RUN pio run -d .


FROM nginx:alpine
COPY --from=build /usr/src/app/.pio/build/esp32doit-devkit-v1/firmware.bin /usr/share/nginx/html/firmware.bin
