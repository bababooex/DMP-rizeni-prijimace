# DMP-rizeni-prijimace
Program a schémata mé maturitní praktické práce. Některé návrhy DPS, popřípadě části programu se dají použít pro jiné účely. Vše slouží pro inspiraci a nemůžu nic garantovat. Knihovny by měly být použitelné i s jinými projekty s STM8S208RB a kompilátorem COSMIC.
# Co všechno program obsahuje?
- Kompletní knihovna na řízení digitálního potenciometru od Adafruitu - DS1841
- Kompletní knihovna na řízení kanálového voliče FM1216 (čip TSA5523)
- Kompletní knihovna na řízení kanálového modulátoru RMUP74055 (čip MBS74T1AEF)
- Kompletní knihovna na řízení satelitního kanálového voliče s čipem AV2026 (v mém programu nepoužito)
- Kompletní knihovna na řízení modulu hodin reálného času s čipem PCF8563
- Knihovna pro maticovou klávesnici s neblokujícím časovým ošetřením stisku
- Upravená knihovna pro 4x40 lcd display se dvěma jednotkami HD44780 (případně ekvivalenty)
- Knihovna pro zobrazení velkého fontu (dvouřádkového) pro lcd display (jenom čísla)
- Knihovna pro řízení rotačních enkodérů softwarově
A další spousta podpůrných knihoven určených specificky pro tento projekt (práce s pamětí, pásmo, vlastní znaky, přepínání režimů...) a samozřejmě hlavní program main
# Co je v KiCadu
- Úplné schéma zapojení hlavní DPS s tlačítkovou DPS, vlastní footprinty a kompletní návrhy obou DPS
