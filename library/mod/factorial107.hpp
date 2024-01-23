#ifndef AJAY_FACTORIAL107
#define AJAY_FACTORIAL107

namespace mitsuha{
// 1<<20
int factorial107table[1024] = {1,55098162,799018112,644524227,804570289,699421653,999080403,347644092,298264049,547915206,68604898,242165296,99769214,860919687,695517422,304751648,800304985,404296372,345504787,346396697,661521818,811907079,150066936,379369971,383295467,935785718,884263687,185573413,564595064,703180737,200891912,45268585,361946029,862561983,555223579,717470752,742784681,818749011,825255985,753131797,774984247,244818236,509057662,323909107,486700580,791867040,303567866,976227856,944323045,513842788,567328309,686789145,441779759,195814622,2372115,135277835,294954750,723496015,119271996,986547466,717213282,842873786,234208469,864111391,117175975,118474328,890000195,504224423,508111147,725796711,802878396,246336953,468479803,310148765,702396285,576460826,175957429,883569154,610938868,287010876,212131504,438039362,359800384,432795594,633410554,155137408,314936343,179257123,943713322,673162662,422288605,545194215,158939936,115503469,689488055,401702717,532814826,592763273,773200138,474494844,496413916,528795782,353056535,193856493,974131920,921613963,393169150,830738748,837700032,174096350,803328680,434966400,698398808,589644499,669724987,860591235,921625212,16363628,511768062,515388641,946983274,375285964,288822198,841563728,550332445,512378200,723231171,688050346,638916270,303773384,338440254,794857710,983752474,388318097,628785057,576616801,25029917,915971206,534292228,562759338,78040824,844961917,391581518,37735563,166114276,533068572,866703952,971782310,839681504,713554826,626185157,514984174,485251798,656102354,587843669,710027943,85325823,3718675,780721779,888259006,709912685,458901728,526677538,873562554,28552679,273046452,694930680,19258350,633408320,497294065,699279305,863781574,941853248,754504516,567820050,153017173,309672260,864251395,999648212,504204977,139941702,735693996,475650352,751284240,61380201,358232497,650878822,427480644,552589593,7500105,216677699,848980694,565479858,273508588,466692797,613411243,300912167,686394608,943827276,838125279,585515687,370926564,947737740,905550006,780384258,985335993,380437817,436417804,952004841,168955240,310513812,254079069,734838326,209059985,189848103,178192696,239063132,724829800,471021218,22656326,940110791,670514782,160800189,60164301,534784668,623973502,444037086,781751482,292944263,109226380,542920902,293050078,501265668,293547492,678045668,870103700,199000138,973807329,712358272,567141781,667220314,930560330,520238050,188798676,915219645,875218552,107463226,924068096,373354885,161687663,742633209,899649882,172223561,490032230,532248036,733461058,864357307,39004186,666796032,19300991,866949601,152723250,632980031,28154382,812475803,342290061,378814433,311792214,811932026,628385664,925729472,483891986,104909388,838245993,226415225,80497756,905742254,760776367,59951296,502463774,432422968,529614143,987845867,941009243,680208181,129640287,135701022,93697263,580002085,736790935,647149348,963929930,456358064,393498474,345303775,836988638,663009951,688270876,821556726,61653598,10436780,40516807,602622268,365808294,624854174,764883550,133418712,979879958,883675628,802594907,960259381,657818185,789571797,869941376,925271608,584915231,373203547,780879573,162303580,219381059,988122582,809692587,524697206,890686281,255441194,208655553,818176738,234316848,355059206,328134956,168206574,937157388,816978580,176422569,755437061,653448585,104372309,277493780,817870252,509736736,467977765,33347068,537441964,30292019,828581950,303478168,255296787,226318316,205744264,27632923,92186650,148892391,626052914,122285044,885244951,369494295,227491286,978201674,280638485,366898179,400916139,936093782,457514504,799178457,77262843,887662440,311502312,433979195,62200437,841399346,399849157,55933275,269778121,314076684,346937429,587781485,403077288,120051495,341462465,804288108,354008902,773347166,273225862,977284051,711403642,332515431,72546287,70463395,383292501,583203296,58684392,59801553,934861184,123659858,172225366,16490486,640104831,651484064,414057597,912563140,516388569,982284977,354948305,845441532,249139343,112170128,852836789,363141564,95948290,593351477,538225430,684921161,437291872,753187673,197482933,175217286,930569788,733325632,206283594,860092876,264612104,375479413,433058282,824726232,663285168,610355794,410260572,141034667,801620565,280447634,539012567,582334448,117111267,192909957,901437820,242191138,473245986,119617826,582524622,27271548,763258578,517785257,740102975,807353604,677357433,502074113,763360936,37227708,928062158,453067630,416392863,403468924,390966467,90780688,97580941,397618185,444801884,718494493,905625902,439144581,809645601,571259258,311262977,379698006,619843354,42160512,709478648,139908674,884941258,7778675,277356393,574397869,64807900,568439116,884254362,793527033,634827367,242270229,166836929,699666005,782871805,430576594,69423864,551852289,854389404,33130302,690600649,389114637,594444305,378027483,501626534,769371462,565727105,185366687,760889594,622679145,531967768,895838088,631099033,636979633,966288648,722632929,487185734,674289231,978197320,42219437,631006005,62801370,477869560,150370443,708309474,569052485,373979365,110651678,526192057,1187964,604094093,92402540,199407377,263950464,85094825,143055746,488295779,242448163,271952450,88191394,640214992,291887361,38760888,143069496,957354294,970267065,791944735,188497194,644438434,778147734,540003575,909162269,307294631,117326469,318009725,133188612,150701170,968756181,75908005,226593925,597645956,881933480,899459287,455981899,229221768,807672432,414097149,643328910,465859747,254429407,523184583,71216457,323329780,538880177,339599190,794248796,185173563,244936068,257855397,449978612,725702638,935779255,177430235,442849413,379319626,300751979,635487273,183449414,5934499,378785317,861919911,678817974,268409923,203403445,572871951,443809175,295389783,505988742,840133940,846932417,746139508,669216033,75861584,858480227,466369788,200782437,410124462,398817445,968382032,980738344,56008451,983178362,179562483,823724361,469703761,906401712,312400795,531915860,313966891,499155799,188116091,542673699,973727253,756116696,555496689,953139148,664729273,301347505,874894370,769419863,39619555,844221994,437468538,567165362,826223750,237919972,889742229,340395341,391452566,470281592,861327653,391319516,846903225,165819062,626164401,316709929,521538790,990712756,973874818,890738343,563978092,109958482,828180512,763297389,963971145,754096813,773609544,459522164,8038398,631562195,353217532,305814226,567542439,895416683,84833664,222068809,137353990,316612885,847273944,686340505,587953096,891163150,312873714,170456823,499030076,552288670,984000707,296655021,907848089,795824037,868241961,731318793,613986016,504817890,831254264,714194680,892153437,432990870,213690618,820549872,68492073,905288570,828076972,100771571,701929895,429147966,855717720,702005759,834355402,271364955,808645569,975796499,767810085,630847407,570582799,453848037,305413582,658578009,72025782,432456806,679768447,209753914,642309814,367985447,648596045,662634665,934081491,20395923,976333508,947778280,290901114,683388002,527020810,977326275,488693383,189698098,419374780,110616851,127872982,575257710,581134348,809887616,616715986,785224788,330000241,549806410,300312979,453241111,120873512,806993146,171910094,251834489,979979771,418698259,752813751,394025833,29105970,80840916,796020902,589114561,204474745,376387134,234987470,80775688,766942711,782094550,251204403,958598099,73582601,425563446,280068465,830973501,760007127,289646156,209940903,843351800,279527279,738627148,205145989,347868746,12854961,648847391,11192482,203518335,83599363,614624895,760355983,241204239,725553388,423530890,233709513,129164025,183143523,474024463,587482677,262947835,830758485,490545965,42715938,774365958,917430364,848838405,184884783,363325662,815621393,13287526,130498889,568838029,994918408,689158356,514272481,755554988,448513470,717322665,827827457,350229871,213613603,814952584,154931117,596054309,996378663,464177580,523761543,498980428,224645330,774166263,755880313,389249660,81743329,776397644,662396499,223245698,916069463,500419874,906869272,990436276,83413835,861165195,955992714,931625913,809913797,990584878,385913422,976567861,988873023,385236984,677028085,265052939,959365279,418677004,258798519,676383755,636025853,270238386,206316099,239739022,885152995,473139022,930824994,355594287,876223850,584874216,450993348,250278273,66731334,677818083,932165316,513087030,595832582,266493735,941688056,358758739,118809938,109331276,935297327,907221850,844856226,966689708,153251680,426805713,872205902,228749156,852770211,253065643,265234802,21716397,785998921,576621647,514060757,602374495,488838351,959753273,105481283,471659303,687019929,675067970,650147251,384820796,572532868,616933893,598368862,955007801,991174700,162133698,58160791,148888347,464964795,922938492,89012579,110574373,388391339,378503165,317247776,8038731,656691478,44251737,780947050,9218625,591467905,660897916,894672548,927151139,575483441,12732642,292282890,535590986,602061256,105810330,549519899,900104648,428472182,404847041,747230327,464453245,284822208,7040213,767161925,268629370,892069143,822121223,971455916,221846561,64553306,769796929,675275231,652246661,449517151,975374186,994429602,299117212,782066823,100306236,683957845,778001329,951091055,742690935,735367617,614080637,290990048,34499833,990980489,559407811,525864614,317820378,806036323,701425752,761690151,688462290,373529632,32050577,719463331,118313951,999322637,97750199,523426443,819087263,545301062,488598090,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int factorial107(long long n) {
    constexpr int mod = 1000000007;
    if (n >= mod) return 0;
    auto [q, r] = div((int) n, 1 << 20);
    long long x = factorial107table[q];
    int s = q << 20;
    For(i, r) x = x * (s + i + 1) % mod;
    return x;
}
} // namespace mitsuha
#endif // AJAY_FACTORIAL107