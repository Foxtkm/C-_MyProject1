for /R %%i in (*.suo) do  (
   del /AH %%i
)

for /R %%i in (UpgradeLog.htm) do  (
   del %%i
)

for /R  %%i in (*.sdf) do  (
   del %%i
)

for /R  %%i in (*.DB) do  (
   del %%i
)

for /R  %%i in (*.user) do  (
   del %%i
)

for /D /R  %%i in (out) do  (
   rmdir /Q /S %%i
)

for /D /R  %%i in (Debug) do  (
   rmdir /Q /S %%i
)

for /D /R  %%i in (Release) do  (
   rmdir /Q /S %%i
)

for /D /R  %%i in (.klocwork) do  (
   rmdir /Q /S %%i
)

for /D /R  %%i in (.vs) do  (
   rmdir /Q /S %%i
)
