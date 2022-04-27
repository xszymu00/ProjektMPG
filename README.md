# ProjektMPG
1. Modelování objektů (3 b) – Alespoň 5 libovolných složitějších útvarů. Ideální je ručně zadat souřadnice vertexů modelovaného objektu jako pole a z tohoto pole vykreslovat přes ukazatel. Do tohoto počtu se nepočítají kvadriky ani vestavěné funkce GLUT.
- [x] 2. Animace (1 b) alespoň jednoho objektu.
- [x] 3. Osvětlení (1 b) – Scéna bude osvětlena alespoň jedním zdrojem světla a předměty budou stínovány. Je třeba zajistit korektní normálování.
- [x] 4. Volný pohyb v horizontální rovině (1 b) pomocí myši a klávesnice. Myší rozhlížení, šipkami (nebo ASDW) pohyb dopředu, dozadu a úkroky do stran.
 - [x] 5. Menu (2 b) obsahující například následující položky (budou-li ve vašem projektu implementovány):
  - [x] a. reset pohybu (nastavení kamery na výchozí souřadnice),
  - [x] b. ovládání animace (zastavení, spuštění),
  - [x] c. zapnutí, vypnutí textur,
  - [x] d. ovládání světla (vypnout, zapnout),
  - [x] e. ukončení programu.
 
- [x] 6. Výpis textu (2 b) – Vypisujte někam do scény pomocí textu poslední vykonávaný příkaz uživatelského rozhraní (rotace, posun, pauza atd.). Použijte funkci glutBitmapCharacter s vámi nastaveným fontem. Text při vykreslování nebude podléhat transformacím a osvětlení – ideální řešení je nastavit matici PROJECTION na ortografické 2D mapování, vykreslit text na patřičné souřadnice a poté nastavit mapování perspektivní a vykreslit vaši scénu. Také při vykreslování textu vypínejte vlastnosti GL_TEXTURE_2D a GL_LIGHTING, aby se neaplikovaly i na něj.
7. Ruční svítilna (2 b) – Na stisk tlačítka R bude rozsvíceno reflektorové světlo s úzkým poloměrem a se středem v pozici kamery.
- [x] 8. Blender model (2 b) – Pomocí Blenderu vymodelujte objekt. Vyexportujte souřadnice vertexů, normál a vložte je do projektu (1 b). Vložte texturu na objekt (1 b).
- [x] 9. Létání (2 b) – Rozšíření pohybu pomocí šipek a myši. Pohyb myší nahoru a dolů způsobí naklonění horizontální roviny pro pohyb.
- [x] 10. Stoupání, klesání (1 b) – Na stisk kláves Page Up/Page Down bude horizontální rovina pro pohyb kamery stoupat nebo klesat.
- [x] 11. Hod předmětu (2 b) – Na stisk tlačítka z pozice kamery vyletí libovolný předmět.
- [x] 12. Simulace kroků (2 b) – Při pohybu v rovině přidejte „nadskakování“ kamery ve směru ke stropu a simulujte tak kroky (1 b). Při zastavení horizontálního pohybu by měl být krok dokončen – uživatel nebude mít pocit, že by zůstal viset nad povrchem (1 b).
13. Tlačítka (2 b) – Část obrazovky bude obsahovat oblasti, které provedou nějakou akci, pokud se do nich klikne myší.
- [x] 14. Průhlednost (1 b) – K jednomu objektu přidejte průhlednost.
15. Projekční paprsek (1 b) – Při stisknutém mezerníku se zobrazí projekční paprsek vycházející ze středu projekce, procházející aktuální pozicí myši a končící zadní ořezovou rovinu.
- [x] 16. Neprůchozí objekt (2 b) – Některý z objektů bude neprůchozí (ze všech stran).
- [x] 17. Texturování (2 b) – Použijte alespoň jednu texturu externě načítanou (1 b) a alespoň jednu texturu generovanou kódem (1 b). Textury musí být odlišné od textur prezentovaných na cvičení.
18. Bézierovy pláty (2 b) – Cílené použití Bézierových plátů ve scéně. Cílené znamená, že plát se nebude jen tak povalovat kvůli splnění bodu, ale bude plnohodnotnou součástí scény, tzn. bude pomocí něj modelován nějaký objekt ve scéně (např. Kaplického knihovna).
