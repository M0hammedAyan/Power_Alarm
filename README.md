# PowerAlarm

Loud alarm + auto-unmute when your laptop is unplugged from AC power. Volume/mute is restored automatically after. Tiny (31 KB), ~0% CPU.

## Install (Windows)

```powershell
git clone https://github.com/M0hammedAyan/Power_Alarm.git && cd Power_Alarm
.\poweralarm.exe
Set-ItemProperty HKCU:\Software\Microsoft\Windows\CurrentVersion\Run -Name PowerAlarm -Value "`"$PWD\poweralarm.exe`""
```

- Run now: `.\poweralarm.exe`
- Last command = auto-start at every login. (Re-run it if you move the folder.)

## Hotkeys

- `Ctrl+Alt+Shift+Insert` — mute/unmute alarm
- `Ctrl+Alt+Shift+F12` — exit app

Startup chime = it's running. Optional build from source: `gcc -O2 -s -mwindows poweralarm.c -o poweralarm.exe -luser32 -lshell32 -lole32 -lwinmm`
