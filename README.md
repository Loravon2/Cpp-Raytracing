# Cpp-Raytracing
## Kompilieren
Das Kompilieren findet jetzt mit Cmake statt mit einem makefile statt. Das ist super, weil ihr dadurch (fast) nichts mehr machen und auch die Libraries nicht mehr selber installieren müsst. Die Schritte sind ganz einfach:
- erst legt ihr einen neuen Ordner "build" an
- dann bewegt ihr euch mit "cd build" in den Ordner
- dort fügt ihr den Befehl "cmake .." aus. Jetzt entsteht in diesem Ordner ein makefile
- dieser kann nun so wie bekannt mit "make" ausgeführt werden um die executable main (bei mir ohne Dateiendung, könnte sich aber unterscheiden) zu erstellen
- ein erneutes ausführen von "cmake .." ist nur nötig, wenn neue Dateien / Bibliotheken hinzugefügt werden. (Diese müssen dann aber auch in CMakeLists.txt erfasst werden). Eine einfache Änderung des Inhalts kann wie bekannt mit "make" kompiliert werden.
## Voraussetzungen
Bitte als aller erstes die gesamte [Erläuterung](https://collaborating.tuhh.de/e-10/teaching/oop/lecture/-/blob/master/5_project/general/README.md#theoretical-description) zum Projekt und insbesondere die sechs technischen [Parts](https://collaborating.tuhh.de/e-10/teaching/oop/lecture/-/blob/master/5_project/general/part1.md) aufmerksam lesen und durcharbeiten!
## Entwicklung
Ich schlage vor, dass wir übergeordnete Issues zu den einzelnen großen Features machen, zu denen wir uns selbstständig zuordnen können (und eventuell durch untergeordnete Issues ergänzen zur besseren Planung falls Bedarf). Bitte stets erst einen Plan machen (ja, auch **vollständige** UML-Diagramme, die brauchen wir für die Dokumentation) und dann implementieren.

Ich fände es zudem praktisch, wenn für jedes Feature einen Branch machen und nach Abschluss mit einem Pull Request auf den main branch holen. Dadurch kann jeder den Überblick über den Zustand des Projekts behalten und Fehler schleichen sich nicht so leicht ein.

Danke fürs Lesen ;D
