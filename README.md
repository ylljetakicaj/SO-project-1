# Distributed System using IPC, Threads, and Synchronization

Ky është një implementim i sistemit të shpërndarë duke përdorur IPC, threads dhe sinkronizim në C në platformën Linux. Sistemi përbëhet nga një server që mund të trajtojë kërkesat nga shumë klientë njëkohësisht, duke komunikuar përmes mekanizmit IPC shared memory. Serveri i kthen të dhënat e kërkesës së klientit në përgjigje dhe demonstron mekanizmat e duhur të sinkronizimit për të siguruar ndarjen e burimeve.

# Implementation Overview
Implementimi përbëhet nga dy komponentë kryesorë: serveri dhe klienti. Programi i serverit krijon një segment memorie të përbashkët për të ruajtur të dhënat e klientit dhe trajton kërkesat hyrëse nga klientët. Ai krijon fije të shumta klientësh për të përpunuar kërkesat në të njëjtën kohë. Serveri përdor mutexes për të mbrojtur burimet e përbashkëta dhe semaforët për të kufizuar numrin e klientëve që mund të lidhen njëkohësisht.

Programi i klientit lidhet me serverin duke përdorur memorie të përbashkët dhe dërgon kërkesa që përmbajnë një lloj kërkese (1-3) dhe një mesazh. Ai merr përgjigje nga serveri dhe i shfaq ato në anën e klientit.

# Build Instructions
1.Sigurohuni që keni Linux me një kompajlues C të instaluar.
2.Klononi depon ose shkarkoni skedarët e kodit burimor në kompjuterin tuaj lokal.
3.Hapni një terminalin dhe shkoni tek directory që përmban skedarët e kodit burimor.

## Server
Kompajloni kodin e serverit me ane te kesaj komande
```
gcc -o server server.c -lpthread -lrt
```

## Client
Kompajloni kodin e klientit me ane te kesaj komande
```
gcc client.c -o client
```
# Run Instructions

## Server
1.Run the server
```
./server
```
## Client
2.Run the client
```
./client
```
3. Ndiqni udhëzimet për të futur llojin e kërkesës (1-3) dhe mesazhin. Shkruani "exit" për të lënë klientin.


