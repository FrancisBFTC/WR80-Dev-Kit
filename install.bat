::setx PATH "%PATH%;%cd%\WDK1.7.0\Tools\WR80\Windows" /M

@echo off
setlocal

:: Diretório do .bat (garantido, mesmo rodando como admin)
set "BASE=%~dp0"
set "NEW=%BASE%WDK1.8.0\Tools\WR80\Windows"

echo Novo caminho a adicionar:
echo %NEW%

:: Ler o PATH atual do sistema com segurança
for /f "tokens=2*" %%A in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path') do (
    set "OLDPATH=%%B"
)

:: Se já existe, não duplica
echo %OLDPATH% | find /i "%NEW%" >nul
if %errorlevel%==0 (
    echo Ja existe no PATH. Nada a fazer.
	pause
    goto :EOF
)

:: Adiciona o novo caminho no final
set "NEWPATH=%OLDPATH%;%NEW%"

:: Grava no registro do sistema sem truncar
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path /t REG_EXPAND_SZ /d "%NEWPATH%" /f

echo PATH atualizado com sucesso!
echo Reinicie o terminal.

pause
endlocal
exit /b

