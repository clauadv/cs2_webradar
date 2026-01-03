@echo off
echo [cs2_webradar] Cleaning build artifacts and dependencies...

REM Clean usermode debug and release folders
echo [cs2_webradar] Removing usermode debug folder...
if exist "usermode\debug" (
	rmdir /s /q "usermode\debug"
	echo [cs2_webradar] usermode\debug removed.
) else (
	echo [cs2_webradar] usermode\debug not found.
)

echo [cs2_webradar] Removing usermode release folder...
if exist "usermode\release" (
	rmdir /s /q "usermode\release"
	echo [cs2_webradar] usermode\release removed.
) else (
	echo [cs2_webradar] usermode\release not found.
)


REM Clean webapp node_modules
echo [cs2_webradar] Removing webapp node_modules...
if exist "webapp\node_modules" (
	rmdir /s /q "webapp\node_modules"
	echo [cs2_webradar] webapp\node_modules removed.
) else (
	echo [cs2_webradar] webapp\node_modules not found.
)

REM Clean webapp dist folder
echo [cs2_webradar] Removing webapp dist folder...
if exist "webapp\dist" (
	rmdir /s /q "webapp\dist"
	echo [cs2_webradar] webapp\dist removed.
) else (
	echo [cs2_webradar] webapp\dist not found.
)

REM Clean webapp dist-ssr folder
echo [cs2_webradar] Removing webapp dist-ssr folder...
if exist "webapp\dist-ssr" (
	rmdir /s /q "webapp\dist-ssr"
	echo [cs2_webradar] webapp\dist-ssr removed.
) else (
	echo [cs2_webradar] webapp\dist-ssr not found.
)

echo [cs2_webradar] Clean complete!
echo Press any key to close this window.
pause