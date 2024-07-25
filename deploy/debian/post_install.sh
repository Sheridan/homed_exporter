#!/bin/bash

if [ ! -f /etc/systemd/system/homed_exporter.service ]; then
  cp /usr/share/homed_exporter/homed_exporter.service /etc/systemd/system/
  systemctl daemon-reload
fi
