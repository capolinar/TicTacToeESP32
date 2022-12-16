:Start
a.exe

Rem This program isn't very smart but it does play tictactoe, just not very well
:loop
for /f "delims=" %%a in (espStat.txt) do (
    set msg = %%a
)
if msg =="Timeout for 30 secs" (timeout /t 30 /nobreak)

if msg == "Run daemon"
(
:loop
echo 1 
echo 2
echo 3
echo 4
echo 5
echo 6
echo 7
echo 8 
echo 9
:end
)

:end