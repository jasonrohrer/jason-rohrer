tell application "Finder" to get folder of (path to me) as Unicode text

set workingDir to POSIX path of result

set myResult to do shell script "cd '" & workingDir & "'; pwd"



# display dialog myResult

do shell script "cd '" & workingDir & "'; /bin/sh ./scripts/mac_before.sh"

do shell script "open -W '" & workingDir & "/bin/Minecraft.app'"

do shell script "cd '" & workingDir & "'; /bin/sh ./scripts/mac_after.sh"


display dialog "Chain World has been successfully saved to USB" buttons "OK"