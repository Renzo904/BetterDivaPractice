# BetterDivaPractice
This mods aims to improve the practice mode integrated in Project Diva: Megamix+


# Features
* Decimal checkpoint position(The original checkpoint system uses integers, so if you try to save in the second 50.5, it will save it to 50 or 51, this makes checkpoints very limited and imprecise)
* "Move Forward" by pressing Shift+F3(or the equivalent in your binds)
* Values configurables from the config.toml

# Configuration
You can configure the mod by modifing the values in `config.toml`
| Parameter      | Description                                                                                                                                                                                                   | Default value                                                                                                  |
|----------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| step_seconds   | The value in seconds the checkpoint will move forwards or backwards                                                                                                                                           | 10(Game's default)                                                                                             |
| restart_offset | Offset between the checkpoint and the point where it restarts, this was created as a way of improving the restart, since most of the time the notes don't appear at the same moment you saved your checkpoint | 1.5(This was set by me, it seems to be the value that is closest to restarting at the same point it was saved) |

# Installation
* Install [DivaModLoader](https://github.com/blueskythlikesclouds/DivaModLoader)
* Download the latest release of this mod from [here](https://github.com/Renzo904/BetterDivaPractice/releases/latest) or from [GameBanana](https://gamebanana.com/mods/482730), and drag the folder `BetterDivaPractice` into the `mods` folder
