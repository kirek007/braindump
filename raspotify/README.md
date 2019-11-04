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

3. Add following groups for use `raspotify` created during installation: `lp audio bluetooth pulse pulse-access`
```bash
sudo usermod -aG bluetooth raspotify
sudo usermod -aG pulse raspotify
sudo usermod -aG pulse-access raspotify 
```
4. Update systemd to start pulseaudio and force speaker connection on start
My file as example:
```bash
[Unit]
Description=Raspotify
After=network.target

[Service]
User=raspotify
Group=raspotify
Restart=always
RestartSec=10
PermissionsStartOnly=true
ExecStartPre=sudo -u raspotify /usr/bin/pulseaudio -D
ExecStartPre=/bin/mkdir -m 0755 -p /var/cache/raspotify ; /bin/chown raspotify:raspotify /var/cache/raspotify
ExecStartPre=sudo -u raspotify bluetoothctl connect XX:XX:XX:XX:XX:XX # Your device MAC here
Environment="DEVICE_NAME=Mirek"
Environment="BITRATE=320"
Environment="CACHE_ARGS=--disable-audio-cache"
Environment="VOLUME_ARGS=--enable-volume-normalisation --linear-volume --initial-volume=100"
Environment="BACKEND_ARGS=--backend alsa"
EnvironmentFile=-/etc/default/raspotify
ExecStart=/usr/bin/librespot --name ${DEVICE_NAME} $BACKEND_ARGS --bitrate ${BITRATE} $CACHE_ARGS $VOLUME_ARGS $OPTIONS

[Install]
WantedBy=multi-user.target

```
