#!/usr/bin/pwsh

$script:Task_ID=""

while (1){
    $(gh pr checks $env:PR_NUMBER).Split([Environment]::NewLine) | ForEach-Object{ 
        if(!$_.contains("Decision Task")){
            Write-Output "no task"
            return;
        }
        if(!$_.contains("pass")){
            Write-Output "not ready"
            return;
        }
        Write-Output $_
        $url = $_.Split([char]9)[-1]
        $script:Task_ID = $url.Replace("https://firefox-ci-tc.services.mozilla.com/tasks/","")
        if($script:Task_ID -eq ""){
            Write-Output "empty"
            return;
        }
        break;
    }
    Start-Sleep -Seconds 60 
}

Write-Output "taskID=$script:Task_ID"
