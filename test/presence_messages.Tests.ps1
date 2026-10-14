$source = Get-Content -Raw (Join-Path $PSScriptRoot '..\lib\business-logic\src\people_presence_alert.cpp')

if ($source -notmatch 'Human face detected') {
    throw 'Missing present-state terminal message.'
}

if ($source -notmatch 'No human face detected') {
    throw 'Missing clear-state terminal message.'
}

Write-Output 'Presence terminal messages are defined.'
