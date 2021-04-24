# Urlcmd Changelog
## v0.4.0

## Version 0.1.0

1. First version of Urlcmd.
2. Convert pseudo-URLs to terminal commands and run them.

## Version 0.2.0

1. Add the ability to preview the resulting terminal commands without
executing them using the flag: `-p`.
2. Add `input-commands` to the help message.

## Version 0.3.0

1. Add flag to switch to the DOS format for commands and flags using `--dos`
or `-d` (e.g. `/?` instead of `-h`).
2. Add flag to skip invalid commands instead of halting Urlcmd. The previous
behaviour Urlcmd had when it encountered an invalid command is to issue
a blank command, which simply skips it but does not identify which
command is invalid. Now, Urlcmd will identify the invalid command if it
encounters one. You can turn on this option using `--skip` or `-S`.
3. Remove rogue "Adding to command list..." verbose output when verbosity
is set to 0.
4. Fix `replaceEscapeCodes` complaints about unterminated escape codes
even though 2 valid hexidecimal digits were given.

## Version 0.4.0

1. Created a new parser for pseudo-URLs. The new parser should allow more
fine-grained error messages and give me more control.
