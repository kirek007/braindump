# How to install raspotify and play using bluetooth speaker

0. Install pulse-audio
1. Install raspotify from https://github.com/dtcooper/raspotify
2. Connect your bluetooth speaker manualy 
- Use bluetoothctl:
```bash
bluetoothctl scan # to get device MAC
bluetoothctl connect xx:xx:xx:xx:xx # Connect to device, keep this line for later
bluetoothctl trust xx:xx:xx:xx:xx # Enable auto connect
```

**Info: I you can't connect to your device be sure to start pulseaudio in advance; other way bluetoothctl can't connect because there is no supported audio profile.**

4. Start pulseaudio server on startup

File: `/lib/systemd/system/pulseaudio.service`
```
[Unit]
Description=Sound Service


[Service]
# Note that notify will only work if --daemonize=no
Type=notify
ExecStart=/usr/bin/pulseaudio --daemonize=no
Restart=on-failure

[Install]
WantedBy=default.target
```

5. Update systemd to start raspotify and force speaker connection

File: `/lib/systemd/system/raspotify.service`
```bash
[Unit]
Description=Raspotify
After=network.target pulseaudio.service bluetooth.service
Requires=pulseaudio.service
[Service]
Restart=always
RestartSec=10
PermissionsStartOnly=true
ExecStartPre=bluetoothctl connect XX:XX:XX:XX:XX:XX
Environment="DEVICE_NAME=BTSpeaker"
Environment="BITRATE=320"
Environment="CACHE_ARGS=--disable-audio-cache"
Environment="VOLUME_ARGS=--enable-volume-normalisation --initial-volume=40"
Environment="BACKEND_ARGS=--backend alsa"
EnvironmentFile=-/etc/default/raspotify
ExecStart=/usr/bin/librespot --name ${DEVICE_NAME} $BACKEND_ARGS --bitrate ${BITRATE} $CACHE_ARGS $VOLUME_ARGS $OPTIONS

[Install]
WantedBy=multi-user.target
```

6. Reload systemd and start service
```
sudo systemctl daemon-reload
sudo service raspotify restart
```

7. Speaker should popup in Spotify client.
