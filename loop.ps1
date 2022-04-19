$runs = 20

for ($num = 1 ; $num -le $runs ; $num++){
    & .\run_send.ps1
    echo "finished iteration: ${num}"
}

$voice = New-Object -ComObject Sapi.spvoice
$voice.speak("Hey David, your genetic algorithm loop is finished!")