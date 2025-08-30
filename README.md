# Paging Simulator
Dieses Projekt ist eine Simulationsumgebung zur Analyse und Visualisierung von Paging-Algorithmen, wie sie in modernen Betriebssystemen zur Verwaltung des virtuellen Speichers eingesetzt werden. Es bietet eine Plattform, um verschiedene Seitenersetzungsstrategien unter identischen, reproduzierbaren Bedingungen auszuführen und zu evaluieren.

<img width="727" height="571" alt="image" src="https://github.com/user-attachments/assets/0d7279c5-1e59-4854-8f62-e545fe13635f" />
<img width="831" height="578" alt="image" src="https://github.com/user-attachments/assets/b04e94ed-5c53-4bbd-979b-d99d2641d0dc" />

Der Simulator ist trace-gesteuert, was bedeutet, dass die Abfolge der Speicherzugriffe aus einer externen Textdatei eingelesen wird. Dies ermöglicht reproduzierbare Testläufe und eine faire Bewertung der Algorithmen.

## ✨ Kernfunktionen
- **Vielfältige Paging-Algorithmen:** Implementiert eine Auswahl gängiger Seitenersetzungsalgorithmen, darunter **FIFO, Second-Chance, LRU, NRU und NFU (mit und ohne Aging)**.
- **Detaillierte Simulationskomponenten:** Bildet die wesentlichen Hardware- und Softwarekomponenten des Paging-Prozesses nach:
  - Physischer Speicher und Seitentabellen.
  - Eine **Memory Management Unit (MMU)**, die einen Translation Lookaside Buffer (**TLB**) zur Beschleunigung von Adressübersetzungen beinhaltet.
  - Ein Kern für die **Diskrete Ereignissimulation (DES)**, um Speicherzugriffe aus einer Trace-Datei sequenziell und zeitlich korrekt zu verarbeiten.
- **Umfassende Analyse:** Bewertet die Algorithmen anhand aussagekräftiger Metriken wie der Anzahl der Seitenfehler, der TLB-Trefferquote und der mittleren Speicherzugriffszeit.
- **Grafische Benutzeroberfläche:** Eine benutzerfreundliche GUI ermöglicht die einfache Konfiguration von Simulationsparametern, die schrittweise Ausführung und die Echtzeit-Visualisierung von Speicher, Seitentabellen und dem TLB.

## 📖 Dokumentation & Präsentation
Für ein vollständiges Verständnis des Projekts, von einem allgemeinen Überblick bis hin zu den technischen Details der Architektur, stehen die folgenden Dokumente zur Verfügung. Sie enthalten alle notwendigen Informationen zu Konzept, Einrichtung und Bedienung des Simulators.

- 📄 **DokumentationUndBedienungsanleitung.pdf:** Das Hauptdokument, das eine detaillierte Projektbeschreibung, die Architekturkonzepte, das API-Design und eine umfassende Bedienungsanleitung für die grafische Oberfläche enthält.

- 📄 **Praesentation.pdf:** Diese Präsentation bietet eine kurze, übergeordnete Zusammenfassung der Projektziele, Merkmale und Ergebnisse.
