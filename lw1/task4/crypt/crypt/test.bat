@echo off

set PROGRAM="%~1"

set OUT="%TEMP%\out.txt"

::Crypt and decrypt the same file
%PROGRAM% crypt "invert.exe" "invert_crypted.exe" 127> %OUT% || goto err
%PROGRAM% decrypt "invert_crypted.exe" "invert_decrypted.exe" 127> %OUT% || goto err
fc invert.exe invert_decrypted.exe || goto err

::Crypt and decrypt the same file GTA5Enhanced
%PROGRAM% crypt "GTA5_Enhanced.exe" "GTA5_Enhanced_crypted.exe" 236> %OUT% || goto err
%PROGRAM% decrypt "GTA5_Enhanced_crypted.exe" "GTA5_Enhanced_decrypted.exe" 236> %OUT% || goto err
fc GTA5_Enhanced.exe GTA5_Enhanced_decrypted.exe || goto err

::Crypt and decrypt the same file 
%PROGRAM% crypt "main.exe" "main_crypted.exe" 236> %OUT% || goto err
%PROGRAM% decrypt "main_crypted.exe" "main_decrypted.exe" 236> %OUT% || goto err

::Key overflow
%PROGRAM% crypt "invert.exe" "invert_crypted.exe" 256> %OUT% || echo Test 1 passed

::Key underflow
%PROGRAM% crypt "invert.exe" "invert_crypted.exe" -1> %OUT% || echo Test 2 passed

::Wrong key format
%PROGRAM% crypt "invert.exe" "invert_crypted.exe" F> %OUT% || echo Test 3 passed

::Input file doesn't exist
%PROGRAM% crypt "invert1.exe" "invert_crypted.exe" 123> %OUT% || echo Test 4 passed

::Crypt and decrypt the same file but with different key
%PROGRAM% crypt "invert.exe" "invert_crypted.exe" 127> %OUT% || goto err
%PROGRAM% decrypt "invert_crypted.exe" "invert_decrypted.exe" 122> %OUT% || goto err
fc invert.exe invert_decrypted.exe || echo Test 5 passed

::Too many arguments
%PROGRAM% crypt "invert1.exe" "invert_crypted.exe" 123 143 1421> %OUT% || echo Test 6 passed

::Too few arguments
%PROGRAM% crypt "invert1.exe" "invert_crypted.exe"> %OUT% || echo Test 7 passed

echo All tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1