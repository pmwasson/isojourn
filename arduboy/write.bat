copy ..\..\isoedit\data.bin data.bin
copy ..\..\isoedit\data.h ..\data.h
python3 flashcart-writer.py -d data.bin

pause
