IF NOT EXIST %cd%/build (
	"make clean make.bat"
) ELSE (
	cd build
	make
	pause
)