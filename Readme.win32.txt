
Galaxy Win - Galaksija Emulator for Windows 
===========================================

v 0.1 - can read DOS Galaxy snapshots now ! (Writes Win version)
        Backspace same as Cursor Left, Delete as Delete (in EDIT mode)
        64k memory now (-8k for ROM) - snapshots are big !
v 0.2 - display is now doubled, unless you use -single option
v 0.3 - Fixed Character display where code >= 192 (Super Cruiser)
        When you Load a snapshot no key is automatically pressed.
        Memory back to 6k RAM because of some incompatibilities
        (Memory expansion will be emulated with the new emulator)

This is a Windows port of Galaxy - Galaksija emulator.
Just a test version not using the buggy Z80Em but instead Marat Fatzulin's 
Z80 Emulation Package. Works like a charm.

This is version 0 - the emulator will be rewritten COMPLETELY in the future,
with TAPE support, Sound, Hi-Res graphics and possible other stuff !

The Snapshot format used by the Windows version is not the same as the
one used by the DOS (registers and more memory)! The saved snapshots will
not be loadable by the DOS version, but you can load the DOS version
snapshots into this one. 

Oh, the emulator has also ROM 2 inside, but so far I haven't found how
to use it (supposadly it has Machine Monitor, new functions, etc.).

If you make Snapshots do make them at the READY prompt so we can save the
memory/etc. to TAPE files later when a new emulator with TAPE support is
ready.

If you have ANY software for Galaksija - on Tape, scanned, snapshots, etc.
then please send it to me - I have nothing so far :(

My Email :

tomcat@sgn.net
**************

Quick Key reference:
--------------------

F2 - Save snapshot (DIFFerent format than Galaxy DOS !)
F3 - Load snapshot

F11 - Show info (FPS, Speed)

TAB - Break (Run/Stop ?)
Alt - Repeat
Scroll Lock - LIST

CTRL-R - Reset
CTRL-N - NMI

CTRL-Q - Quit

Command line options :
----------------------

-single       Use single sized pixels (otherwise double size used)
-display x y  Use resolution x*y (windowed)
-sync n       n=0 (as fast as it gets), 1 (50 Hz), >=10 skip frames
-ssaver n     Start Screen Saver after n minutes (default 15)
-realblack    Use real BLACK colour for Background (default is GREY)

There is a zip file with full DOS version in this package!

This is the readme file of the DOS version in Serbo-Croatian ! Some changes
were made where the Windows version differs !

Sadrzaj

    1.0 Copyright napomena
    2.0 Uvodna rec

    3.0 Galaksija ? Sta to bese ...

        3.1 Kratka istorija GALAKSIJE
        3.2 Mogucnosti GALAKSIJE

    4.0 Sta je emulator, i kako to ustvari radi ?!?

        4.1 Koliko brza masina je potrebna za punu brzinu emulacije.
        4.2 Pustanje u rad
        4.3 Opcije prilikom startovanja emulatora
        4.4 Radno okruzenje
        4.5 Opcije unutar radnog okruzenja

            4.5.1 Reset
            4.5.2 Hard brejk iliti NMI taster
            4.5.3 Napustanje emulatora, odnosno povratak u DOS, WINDOWS (3.1-NT4.0), Linux, ...
            4.5.4 Snimanje programa
            4.5.5 Ucitavanje programa
            4.5.6 Statusna linija
            4.5.7 Screen saver iliti zastitnik fosfora Vaseg ekrana

        4.7 Povecanje brzine emulacije prirucnim metodama

    5.0 Par reci o kompajliranju izvornog koda.
    6.0 Zavrsna rec
    7.0 Napomene
    8.0 Literatura
    9.0 Hvala ...
   10.0 Kako me mozete kontaktirati ...






1.0 Copyright napomena

    Nad ovim programom imaju kopirajt sledece osobe:

    Miodrag Jevremovic  Autor samog emulatora.
    Shawn Hargreaves    Tvorac sada vec legendarne biblioteke ALLEGRO.
    Marcel de Kogel     Da nije Z80 C emulatora, tesko da bih ista poceo.

    Za dodatne informacije molim Vas pogledajte datoteku '(c)right'


2.0 Uvodna rec

        O cemu bih u ovom delu mogao pisati nego o stvaranju ideje za radjanje
    jednog ovakvog projekta. Pa, evo ovako. Posle kraceg koriscenja 'YAHOO'-a
    i 'ALTA-VISTE' primetio sam da se emulatorska scena odprilike od jeseni
    prosle godine (1996.) razvijala rapidno brzo. Zasto? Kao i uvek, stvorili
    su se uslovi za pokretanje lancane reakcije. Jedan od bitnijih uslova
    je drasticno pojeftinjenje hardvera, odnosno poprilican skok performansi
    PC racunara. (Normalno govorim o zapadu.) Naime, prvi popularniji
    emulatori (ZX48K, CBM 64) su pisani jos tamo negde 92-93. god. Pisani su
    vecim delom u asembleru, i u principu im je za 100% emulaciju bila
    dovoljna i 286-ca na 16-20MHz !?!. ALI, (uvek ima neko ali) takvi
    programi nisu bili ni u kom slucaju portabilni. Dakle nista od Unix-a,
    Macintosh-a,itd. Drugo, ipak su bili pisani u asmebleru, medjutim
    sta ako ga neznate, ima li smisla savladavati ga samo zbog jednog
    emulatora ?!? Sem pomenutog pojeftinjena/poboljsanja performansi drugi
    nista manje bitan uslov je bio Marcel de Kogel-ov Z80 emulator, ciji je
    izvorni kod mogao skinuti ama bas svako. Tu je bio i ALLEGRO, koji je
    u principu sadrzao SVE sto Vam je moglo zatrebati da uradite emulator
    bilo koje masine. Izvorni kod ALLEGRO biblioteke je takodje bio potpuno
    besplatan. Ta tri uslova, i desio se BIG-BANG na emulatorskoj sceni.
    Emulatori su poceli da nicu kao pecurke, jer je sada, za njihovo
    pravljenje trebalo samo napisati emulaciju hardvera emulirane masine.
    Normalno, takvi programi, koji su ipak 100% uradjeni u C-u, su bili
    neuporedivo sporiji nego da su bili napisani na asembleru. Ali, brzine
    masina stalno napreduju, tako da ako vam danas ne radi bas nesto preterano
    brzo, za nepunih godinu-dve cete najverovatnije imati dovoljno snaznu
    masinu na kojoj ce sasvim lepo raditi. I Z80em i ALLEGRO, se mogu vrlo lako
    prebaciti na bilo koju masinu na kojoj postoji C kompajler, i koja
    koristi klasican VGA. To su zapravo SVE masine koje su danas u upotrebi.
    I tako, ljudi su pisali i pisali i za proteklih skoro godinu dana,
    napisano je skoro SVE ! Sve, ali ne i GALAKSIJA emulator. Jer ako postoje
    emulatori masina za koje vecina Vas nije u zivotu cula, zasto ne
    napraviti emulator naseg PRVOG i jedinog KUCNOG racunara u samoizradi ?!?
    Uuuhhhhh ...


3.0 Galaksija ? Sta to bese ...

        Nas (Drugo - Jugoslovenski) kucni prvenac. Pravljen je iskljucivo od
    delova koji su mogli da se nadju u maltene svakoj prodavnici elektronskih
    delova. (Normalno na zapadu. Kod nas ih nikada nije ni bilo, nema ih a
    bogami NIKADA ih i nece biti.(Mislim na prodavnice)) I tako, bili ste
    prinudjeni ili da narucujete komplet delova, do koga se poprilicno tesko
    dolazilo ili pak da obilazite otpade u potrazi za nekom masinom. Jedan
    od omiljenih nacina dobijanja delova je bio preko Tetke/Ujaka iz recimo,
    Amerike. ;)
        Srce masine je bio Z80, pamet mu je zavisila od popunjenosti mesta
    za pamet. Ona stalna je mogla biti 4k odnosno ako ste zeleli da vam
    racunar bude bas pametan cak 8k (ROM-a). Dok je ona, privremena bila od
    2k do 6k. (RAM-a). Tastaturu, ekran i deo za kasetofon opsluzivao je Z80.
    Brzina mu je bila 3,072,000Hz, tacno pola (zamislite!) od 6,144,000Hz
    koliko je bilo potrebno delu elektronike namenjenom generisanju slike.
    Tu je jos bilo i 2k EPROM-a, unutar koga je bila definicija svih
    galaksijinih znakova, sa sve karakterima za generisanje grafike. Ukoliko
    Vas bas interesuje kako je to izgledalo, pronadjite casopis RACUNARI br. 1
        Kao sto ste primetili neko osnovno poznavanje racunarskih termina i
    samog rada racunara se podrazumeva.


3.1 Kratka istorija GALAKSIJE

        O GALAKSIJI je prvo pisano unutar jel'te casopisa GALAKSIJA, koji je
    u to doba (82-85) bas radio na popularizaciji racunara kod nas. Kasnije,
    1984., izlazi specijalno izdanje posveceno kucnim racunarima 'RACUNARI'.
    Unutar tog prvog broja bila je sema galaksije, stampane ploce, kao i
    kompletno uputstvo za samogradnju. Casopis RACUNARI je nastavio sa zivotom
    i u njemu je kasnije dosta pisano. Pokrenuti su projekti programatora
    eproma, zvucne kartice, prosirenja memorije, visoke rezolucije, sto je
    uz dodatke iz drugih casopisa, recimo 'SVET KOMPJUTERA' na kraju preraslo
    u projekat GALAKSIJA +. To je trebao da bude nas zvanicni skolski racunar
    ALI, nije. Galaksija je pominjana jos par godina, i o njoj je dosta pisano
    pogotovu u SK, gde je objavljeno i najvise listinga poprilicno dobrih
    programa i igara, pisanih u masincu. Galaksija je postojala tamo negde do
    86' - 87' i to je bio njen kraj. Medjutim cinjenica je da je to malo
    racunarce napravilo preko 7000 samograditelja, koji su uz to sklapanje
    GALAKSIJE i poprilicno naucili. (Barem se nadam da su naucili.)
        Verovali ili ne prosle (1996.) godine mi je dosao covek sa molbom da
    mu pomognem oko ozivljavanja njegove GALAKSIJE !?! Eto,...


3.2 Mogucnosti GALAKSIJE

        Da se razumemo, GALAKSIJA je bila bazirana na Z80 procesoru, koji je
    provodio i do 70% svog vremena na generisanje slike i ocitavanje
    tastature. S' toga nije joj nesto mnogo vremena preostajalo za operativni
    , korisniku 'opipljivi' deo. Bejzik jeste bio skroman, i relativno brz (u
    odnosu na tadasnje masine) ali SVE to je smesteno u samo 4k. Probajte da
    uradite tako nesto pa cete videti o cemu se radi. Bilo je dosta prosirenja
    i mogli ste zacas da je otvorite, zalemite zicku tamo, zicku ovamo, i Vasa
    GALAKSIJICA dobi neko novo parce harvera. Dakle NAJOTVORENIJI racunar na
    svetu. Zivela GALAKSIJA !!! O njoj se znalo SVE. Ostalo je, sve na Vama.
       Primenjeni hardverski minimalizam i lepota softvera su prosto
    odusevljavali svakog pravog zaljubljenika u racunarsku tehniku.


4.0 Sta je emulator, i kako to ustvari radi ?!?

        Program koji u potpunosti simulira to sto zmo izabrali da oponasamo.
    Dakle, startovanjem ovog programa, efektivno dobijate racunar GALAKSIJA.
    Na ekranu dobijate ono sto bi ste dobili na TV-u. Tastatura Vaseg PC-a,
    pocinje da radi kao tastatura GALAKSIJE, i kao vrhunac svega velika
    vecina programa ce biti u potpunosti ispravno izvrsena. Nakon izlaska iz
    emulatora, PC nastavlja sa svojim uobicajenim poslovima.
        Kako radi? Prosto. Imamo program koji u potpunosti imitira mozak tj.
    procesor Z80. Takodje imamo i originalne galaksijine ROM-ove. Ostalo je
    na nama. Kada GALAKSIJA pokusa da nesto ispise na ekranu, mi to primetimo
    i ispisemo na monitoru. Kada pritisnemo neki taster, informaciju o tome
    prosledimo GALAKSIJI kao da je taj kontakt ostvaren preko prave
    GALAKSIJINE tastature. Sve u svemu pomocu ovog programa se trudimo da
    GALAKSIJI okolina izgleda potpuno normalno, iako uopste nista od te
    okoline uopste i ne postoji. Moram da napomenem i to da pod pojmom
    'GALAKSIJA' u ovom delu podrazumevam ROM1, u kome su OS i bejzik.


4.1 Koliko brza masina je potrebna za punu brzinu emulacije.

    Da bi ste ostvarili 100% brzine galaksije, morate imati sledece:

    a. Pentium na 100 ili brze. (Eventualno 486DX4 ali uz ispunjen uslov 'b')
    b. Sto je moguce brzi pristup memoriji graficke kartice. Tj., makar VLB.
    v. Makar 1Mb slobodnog prostora na disku.
    g. Makar 1Mb slobodnog RAM-a.
    d. DOS (A, ako poznajete C, i imate drugu platformu, onda i na njoj.)


4.2 Pustanje u rad

        Napravite direktorijum, u njemu raspakujte arhivu koristeci prekidac
    -d, i to bi bilo to. Nakon toga da bi ste napokon, posle svih ovih godina
    ponovo napisali koje programce u Galaksijinom bejziku, sve sto treba da
    uradite je GALAXY+enter.

        Evo:

        1.      md galaxy + enter
        2.      cd galaxy + enter
        3.      pkunzip -d a:galaxy + enter
                (Umesto a: navedite punu putanju do arhive.)
        4.      galaxy + enter

        ... ili slicno. Ovde se podrazumeva da vam je galaksija zapakovana u
        arhivu GALAXY.ZIP i da se nalazi na disketi A:.


4.3 Opcije prilikom startovanja emulatora

        O tome kako ce da izgleda radna okolina GALAKSIJE odlucujete sami.
    Parametre namestate prilikom pustanja emulatora u pogon. SVI parametri
    pocinju sa znakom '-' minus, iza koga neposredno (bez razmaka) sledi
    naziv opcije, i eventualno iza nje UZ 1 RAZMAK sledi i parametar (ili 2).
    Parametri MORAJU biti otkucani MALIM slovima.

    Opcije su:

    -display parametar (sirina visina)

        Ovom opcijom odlucujete u kojoj rezoluciji cete raditi.
        Ako ne dodate -single opcije onda ce se rezolucija podvojiti !
        (pikseli ce biti udvostruku veliki)
        Emulator za Windowse UVIJEK radi u Windowed nacinu - u oknu !

        Primer upotrebe:

        -display 256 224

        Galaksijina slika je bila 256x224, ukoliko izabrani mod ima neku od
        ovih osa manju nego GALAKSIJINA, sve ce lepo raditi, ali ce normalno
        jedan deo slike ispasti van ekrana.

    -single
    
        Ne podvoji piksele (pikseli su normalno veliki) - ako imate malu
        rezoluciju na desktopu onda upotrebite ovu opciju.

    -sync parametar

        Ovom opcijom uspostavljate kakav takav red u ispisivanju slike na
        ekranu. Dakle da li ce se slika iscrtavati u pravilnim vremenskim
        intervalima.

        Parametri su:

        0 ----- Nema sinhronizacije. Slika se iscrtava onoliko puta koliko
                je brza Vasa masina.

        1 ----- 50Hz ALI vestacki generisanih, tj. preko ALLEGRO-vog tajmera.
                Ne valja sto je frekvencija osvezavanja slike na Vasem
                racunaru neka sasvim druga (veca) vrednost. Tj. od finog
                skrola nista, ali radi u svakom modu i dobija se 'NORMALNA'
                GALAKSIJINA brzina. 3.072MHz !!!

        2 ----- Vrsi se sinhronizacija ispisa GALAKSIJINIH ekrana i ispisa
                PC ekrana. Dobija se savrseno fino skrolovanje, ALI ce Vam
                GALAKSIJA uglavnom raditi brze, zato sto ni jedan normalan
                graficki mod nema frekvenciju osvezavanja ekrana 50Hz.


        >=10 -- Definisete sopstvenu frekvenciju osvezavanja GALAKSIJINOG
                ekrana.


        Primer upotrebe:

        -sync 2


    -ssaver parametar

        Nakon nekog vremena pojavice se efekat blagog zatamnjenja i ponovnog
        odtamnjenja, takozvani FADE IN / FADE OUT. Vise info o cuvaru ekrana
        u odeljku 4.5.7.

        Parametar je vreme u MINUTAMA koje mora da istekne od poslednjeg
        pritiska na bilo koji GALAKSIJIN taster. Izlazak iz cuvara ekrana
        je na pritisak bilo kog GALAKSIJINOG tastera. Cuvar ni na koji nacin
        ne remeti normalno funkcionisanje emulatora, tj. izvrsavanje
        GALAKSIJINIH uobicajenih poslova.

        Primer upotrebe:

        -ssaver 10


    -realblack

        Opcija sluzi iskljucivo radi sto vernijeg docaravanja rada za TV-om.
        Bez ove opcije, namestio sam da GALAKSIJINA boja 'papira' ne bude
        bas crna, vec malkice iznad, tj. siva. Zasto ? Pa da bi se bolje
        video border. Ukoliko zelite da sve bude crno, kao na TV-u, ubacite
        ovu opciju.

        Primer upotrebe:

        -realblack


        DA SUMIRAMO !!!

        Zelite mod 400x300, sinhronizaciju ispisa GALAKSIJINE slike sa
        izabranim grafickim modom, aktiviranje cuvara ekrana nakon 5 min. i
        crninu, to bi izgledalo ovako :

        GALAXY -display 400 300 -sync 2 -ssaver 5 -realblack        + ENTER


4.4 Radno okruzenje

    izgleda ovako. Prva linija, koja se po startovanju emulatora ne
    prikazuje, sadrzi informacije o trenutnom broju ispisanih slika u
    sekundi, kao i trenutnu brzinu GALAKSIJE. Ostalih 16 linija X 32
    kolone je GALAKSIJIN ekran.
    Tastatura je prilagodjena originalnoj koliko je to bilo moguce
    izvesti. LEVI ALT je RPT, TAB je BREAK, ostalo pogodite sami.


4.5 Opcije unutar radnog okruzenja

    U izvesnoj meri mozete uticati na radno okruzenje GALAKSIJE, evo sta
    i kako mozete raditi ...


4.5.1 Reset

    Kao i na svakoj tadasnjoj masini postojao je taster RESET. Bez njega,
    tj. njegove ugradnje, bogami tesko da bi se mogao zamisliti neki rad
    za racunarom. (Barem pri prvim pseudo-naprednim kontaktima sa
    racunarom.) Taster se vrlo lako ugradjivao. Izvode tastera spojite
    na masu i nozicu RESET od Z80 i, to je to. Pritiskom na taster
    dolazilo je do 'inicijalizacije' procesora, tj. prevedeno na domaci
    jezik, racunar se nakon pritiska na taster ponasao kao da smo ga
    upravo ukljucili. Dakle, gubio se sadrzaj RAM-a, tj. svega sto smo
    uneli. Desavalo se naime, da se napise program u asembleru, startuje
    se, i slog. U tom slucaju RESET je bio neizbezan. Uostalom nije morao
    da se sokira racunar stalnim gasenjem i paljenjem, efekat je bio isti.
    U ovom emulatoru, racunar resetujete pritiskom na tastere CTRL i R !!!


4.5.2 Hard brejk iliti NMI taster

    Slicno RESET tasteru. Razlika je u tome da, prilikom pritiska na ovaj
    taster postoji sansa da je nas program jos uvek u memoriji, tj. da
    mozemo da nastavimo sa uobicajenim radom. Glavna namena tastera je
    bila zapravo nasilno prekidanje 'mrtvih petlji'. U emulatoru, NMI
    taster dobijate pritiskom na tastere CTRL i N !!!


4.5.3 Napustanje emulatora, odnosno povratak u DOS, WINDOWS (3.1-NT4.0), Linux, ...

    Najprostiji deo. Pritisnite CTRL i Q, i izadjoste. Samo, jel' snimiste
    ono sto ste unosili ?!?


4.5.4 Snimanje programa

    Da bi ste sacuvali svoj upravo, mukotrpno, odnosno jedva unet listing,
    pritisnite taster F2 i to bi bilo to. Programi se snimaju kao
    'SNAPSHOT' galaksijinog RAM-a, odnosno snimi se kompletno stanje
    GALAKSIJINOG RAM-a + trenutno stanje SVEGA unutar Z80. (Moze nam se!)
    Mana ovakvog pristupa je nemogucnost MERGE-ovanja programa, ali
    ukoliko bude bilo dovoljno interesovanja, mozda u sledecoj verziji.


4.5.5 Ucitavanje programa

    Isto kao kod snimanja. Dakle radi se o ucitavanju kompletnog sadrzaja
    RAM-a + stanje svega unutar Z80. Vrsi se pritiskom na F3.


4.5.6 Statusna linija

    Prva vidljiva linija na monitoru. (Normalno ako je aktivirana.) Dobija
    se pritiskom na F11. (F11 je taster iz M.A.M.E.-a) Sadrzi samo dve
    informacije. Prva je broj ispisanih slika na ekranu, a druga info je
    trenutna efektivna brzina u Hz-ima. Broj ispisanih slika ?!? Da, na
    taj nacin vidite da li Vasa masina moze da izgura punih 50Hz koliko
    iznosi 100% brzine iscrtavanja slike na pravoj GALAKSIJI. Ukoliko je
    manje, ili sacekajte novu plocu, ili probajte sa nekom malkice brzom
    VGA kartom. Bitno je da bude brza, a sto se mene tice dovoljno je da
    ima makar 256k na sebi. Eto, statusna linija je tu radi Vaseg sto
    boljeg informisanja. Kako se aktivira, tako se i deaktivira, na F11.
    Mozete je pozivati u bilo kom trenutku, jer radi pod IRQ-om, tako da
    ni malo ne utice na rad emulatora.


4.5.7 Screen saver iliti zastitnik fosfora Vaseg ekrana

    Tu je iskljucivo da bi spasao fosfor Vaseg monitora. Jer, ukoliko je
    na ekranu duze vreme staticna slika, na mestima gde je jak snop
    svetlosti dolazi do ostecenja fosfora. Malo po malo i posle par
    godina na ekranu imate sliku i kad ugasite monitor. Sta kazete, to i
    nije tako lose, ALI tu sliku moci cete da uklonite samo ako, jel'te
    kupite nov' monitor. ;)


4.7 Povecanje brzine emulacije prirucnim metodama

    Sem kupovine nove ploce, brze karte i kojekakvih navijanja istih,
    emulator mozete ubrzati koristeci odgovarajuce izabrane parametre iz
    komandne linije.

    Najveci deo emulacije otpadne (kao i kod GALAKSIJICE) na ispis slike.
    Dakle ukoliko ne mozemo da doguramo do onih punih 100% tj.
    SINHRONIZOVANIH 50Hz, mozemo probati sa :

    1.  Ispisom slike bez ikakve sinhronizacije sa aktuelnim grafickim
        modom. Dakle kad GALAKSIJI, stigne zahtev za generisanje slike,
        slika se izgenerise i nastavlja se dalje, bez cekanja sinhro
        inpulsa.

        Dakle: GALAXY -sync 0

        ... pa koliko ispadne, ispadne.

    2.  Ispisom slike sa manjim FPS-om. Na taj nacin se svesno odricemo
        kvaliteta ispisa slike (kod finih skrolovanja ekrana), na racun
        Z80 jezgra.

        Dakle: GALAXY -sync 15

        Menjajte cifru dok ne dobijete priblizno nekih 3012000.00 Hz ;)

    Na zalost unutar emulatora nije (iako postoji ta mogucnost) ugradjena
    podrska za VESA 2.0 linearni pristup video memoriji. Najverovatnije
    bi ubrzalo donekle ispis, ali jednostavno mislim da nije isplativo
    baktati se oko toga. Na mojoj AMD P5-133 masini to jako lepo radi
    i bez toga, a cene tako brzo padaju, ...


5.0 Par reci o kompajliranju izvornog koda.

    DJGPP je port GNU C-a na DOS platformu. S' obzirom da je freeware, stekao
    je poprilicnu popularnost. Osim toga generise cistokrvne 32-o bitne
    aplikacije pod DOS-om. Dakle, bafer=malloc(10000000) !?! Osim toga, vecina
    autora emulatora pribegava njemu, zato sto za njega imate emulatore
    maltene svih procesora, normalno u freeware obliku, a tu su i ALLEGRO,
    SEAL,... !!! Razlika izmedju ovog i bilo kog drugog C-a skoro i da nema.
    Dobar deo normalno pisanih programa, koje sam radio u BCC-u bez ikakvog
    problema prevedoh pod ovim C-om. Takodje kod pisan u njemu je maksimalno
    portabilan. GNU C postoji prakticno na svakoj platformi, a kada kao u ovom
    slucaju imate izvodni kod za sve i svasta, zakljucite sami sta iz svega
    toga ispada. Ukoliko se odlucite da instalirate ovaj C, prvo skinite sa
    3-ceg linka datoteku 'readme.1st', kako bi ste znali sta i kako da
    uradite prvo. (Koje datoteke su sta, Instalacija, podesavanje, itd.)

    Linkovi:

    1.  Program je kompajliran pod DJGPP-om.
            http://www.delorie.com/djgpp/

    2.  Z80Em mozete naci na Marcel de Kogel-ovoj strani.
            http://www.komkon.org/~dekogel/

    3.  ALLEGRO, DJGPP i puno izvornog koda za DJGPP mozete naci na recimo,
            ftp.demon.co.uk/pub/simtelnet/gnu/djgpp/


6.0 Zavrsna rec

    Cetri slova, ... ,Kraj.

    Salu na stranu, ovoliko od mene. Imate moj e-mail, pa ukoliko Vam
    nesto bas nije po volji, kontaktirajte me. Ukoliko bi ste da nesto
    dodate, izmenite ili poboljsate, takodje me kontaktirajte. Nemojte se
    ljutiti ako bas Vama ne odgovorim, ukoliko Vam je bas bitno pokusajte
    ponovo.

    A, ukoliko se pitate cemu sve ovo, nije ni bitno, obrisite emulator sa
    Vaseg diska i zaboravite.


7.0 NAPOMENE

    1.

    Imajte u vidu da, kada se pozovu SAVE (F2) ili LOAD (F3), emulator STAJE.
    Dakle u pozadini galaksija NE RADI sve dok ne izadjete iz pomenutih opcija.
    Bitno je zbog slucajeva gde bi se mogao koristiti GALAKSIJIN sat !

    2.

    Iskrice koje se mogu pojaviti oko slova su rezultat sledeceg. Dakle, da bi se
    uradio FADE-in/out, paleta se mora polako 'gasiti' odnosno 'paliti'. E' sad' !
    Rutina iz ALLEGRO-a je to radila nakon VSYNC-a. S obzirom da se ista pozivala
    iz interapta dovodila je do nezeljene sinhronizacije za frekvencijom
    osvezavanja izabranog moda. Moja rutina (ista kao i originalna) jednostavno
    samo nema cekanje na VSYNC, vec to uradi odmah. Upravo zbog tog necekanja
    dolazi do varnicenja po ivicama znakova.


8.0 Literatura

    Prvi i za sada jedini clanak koji se bavio priblizno ovom tematikom,
    (emulatorima, simulatorima) bio je u casopisu RACUNARI, clanak je
    potpisao, a ko bi drugi do, gospodin Dejan Ristanovic.


9.0 Hvala ...

    Gospodi:

    Marcel de Kogel         na jezgru Z80 emulatora.
    Shawn Hargreaves        na ALLEGRO-u.
    Predragu Djordjevicu    na svemu. Bez njega tesko da bih znao ovoliko o
                            racunarima.
    Zoranu Jovkovicu        na podrsci.
    Dejanu Ristanovicu      na profesionalno napisanim clancima.
    Voji Antonicu           na stvaranju GALAKSIJE.


10.0 Kako me mozete kontaktirati ...
                     ,
    MIODRAG JEVREMOVIC
    Petra Ilica 1/14
    37000 Krusevac

    [21.20E, 43.35N]
    Yugoslavia.europe.EARTH...
    ...HumanSolarSystem.MilkyWay.EMULATOR-SPACE-PROJECT.          ?

    amidar@EUnet.yu
