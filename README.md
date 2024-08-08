# MakerFridge

A fridge made by makers for makers.

## SecDevOps

### Credentials management

Credentials are stored in a `.env` file similar to `env.example` in your local repo that should never be uploaded to version control.

However, we need to put them in the `platformio.ino` file in order to use them during builds.

Due to the limitations from PlatformIO, we cannot just reference shell variables.

Instead before adding changes, call `scripts/rm_credentials.sh` to remove credentials in `platformio.ino`, and when your commit is done you can restore them calling `scripts/set_credentials.sh` from the variables set in your environment.

Ideally this should be automated as a git hook.
