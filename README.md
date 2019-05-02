# ready-to-command

Run a custom command with a countdown prompted in GTK3 GUI.

## Usage

```
Usage:
  ready-to-command [OPTION…]

Help Options:
  -h, --help                 Show help options
  --help-all                 Show all help options
  --help-gapplication        Show GApplication options
  --help-gtk                 Show GTK+ Options

Application Options:
  -t, --command-title        The title of the command.
  -d, --command-desc         The description of the command.
  -a, --command-action       The action description of the command.
  -e, --shell-exec           The actual shell command of the command.
  --display=DISPLAY          X display to use
```


## Use Case

### *Ready to* Hibernate

```bash
./ready-to-command -t "Hibernate" -d "Your computer is about to hibernate" -a "hibernation" -e "systemctl hibernate"
```

![](https://github.com/chienius/ready-to-command/raw/master/ready-to-hibernate.png)