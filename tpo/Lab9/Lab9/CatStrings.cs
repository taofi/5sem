using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab9
{
    public static class CatStrings
    {
        public static readonly string[] CatGameDescriptions = new string[]
        {
            "Кот, как ураган, носится за лазерной точкой, озаряя комнату энергией.",
            "Он взлетает на когтеточку, словно герой, готовый покорить вершину.",
            "С грацией охотника он бросается на игрушечную мышь, демонстрируя мастерство.",
            "Кувыркаясь на полу, кот весело бьёт лапой по пушистому перышку.",
            "С триумфальным видом кот уносит игрушку, словно это его трофей."
        };

        public static readonly string[] AngryGameReactions = new string[]
        {
            "Кот бросается на игрушку с диким рыком, словно это его заклятый враг.",
            "Его когти впиваются в игрушку, а взгляд горит яростью охотника.",
            "Кот шипит и бьёт игрушку лапами так, будто собирается разорвать её на куски."
        };

        public static readonly string[] PositiveReactions = new string[]
        {
            "Кот мурлычет так громко, что кажется, будто он поёт свою счастливую песню.",
            "Он трётся об вашу руку, мягко намекая, что хочется ещё немного ласки.",
            "Кот падает на спину и показывает живот, доверяя вам больше, чем миру."
        };

        public static readonly string[] NegativeReactions = new string[]
        {
            "Кот угрожающе шипит и быстро отталкивает вашу руку, будто защищая себя.",
            "Его взгляд становится колючим, а хвост злостно машет из стороны в сторону.",
            "Кот молча разворачивается и, презрительно задрав хвост, уходит в другую комнату."
        };

        public static readonly string[] FoodReactions = new string[]
        {
            "Кот с аппетитом поглощает еду, его усы счастливо подрагивают.",
            "Он ловко грызёт кусочек, словно это его самый вкусный обед.",
            "Кот лижет миску, не оставляя ни крошки, и смотрит на вас с надеждой."
        };

        public static readonly string[] WaterReactions = new string[]
        {
            "Кот пьёт воду с грацией, тихо причмокивая.",
            "Он аккуратно наклоняется к миске и жадно утоляет жажду.",
            "Капельки воды сверкают на его носу, пока он доволен свежей прохладой."
        };
        public static readonly string[] CatIdleActions = new string[]
{
    "Кот мирно спит, тихо посапывая во сне.",
    "Кот сидит, неотрывно глядя на проходящих мимо людей.",
    "Кот смотрит в окно, изучая прохожих и машин, словно наблюдатель.",
    "Кот внимательно наблюдает за мелкими духами, завороженно следя за их движениями.",
    "Кот ищет что-то интересное под диваном, настойчиво вытаскивая игрушки.",
    "Кот лениво потягивается, пробуждая в себе энергию для следующей игры.",
    "Кот уединяется в своем уютном уголке, погружаясь в мир своих мыслей."
};

        public static readonly string[] catAsciiArt = new string[]
 {
    @"
    /\_____/\  
   /  o   o  \ 
  ( ==  ^  == )
   )         ( 
  (           )
 ( (  )   (  ) )
(__(__)___(__)__)
",
    @"
 _._     _,-' ''`-._
(,-.`._,'(       |\\`-/| 
    `-.-' \  )-`( , o o)
          `-     \`_`''- 
",
    @"
  ,-.       _,---._ __  / \ 
 /  )    .-'       `./ /   \
(  (   ,'            `/    /| 
 \  `-'            '\ \   / | 
  `.              ,  \ \ /  |
   /`.          ,'-`----Y   | 
  (            ;        |   ' 
  |  ,-.    ,-'         |  / 
  |  | (   |            | /  
  )  |  \  `.___________|/   
  `--'   `--' 
",
    @"
      |\\      _,,,---,,_ 
ZZZzz /,`.-'`'    -.  ;-;;,_ 
     |,4-  ) )-,_. ,\\ (  `'-' 
    '---''(_/--'  `-'\\_) 
"
};
        private static readonly int maxWidth = 34; // Максимальная ширина
        private static readonly int maxHeight = catAsciiArt.Max(art => art.Split(new[] { '\n' }).Length); // Максимальная высота

        private static string NormalizeArt(string art)
        {
            string[] lines = art.Split(new[] { '\n' }, StringSplitOptions.None);

            // Определяем, сколько пустых строк нужно добавить
            int totalEmptyLines = maxHeight - lines.Length;
            int linesToAddTop = totalEmptyLines / 2; // Количество пустых строк сверху
            int linesToAddBottom = totalEmptyLines - linesToAddTop; // Количество пустых строк снизу

            // Создаем список с пустыми строками
            string[] emptyLinesTop = new string[linesToAddTop];
            string[] emptyLinesBottom = new string[linesToAddBottom];
            for (int i = 0; i < linesToAddTop; i++)
            {
                emptyLinesTop[i] = new string(' ', maxWidth); // Пустая строка с пробелами
            }
            for (int i = 0; i < linesToAddBottom; i++)
            {
                emptyLinesBottom[i] = new string(' ', maxWidth); // Пустая строка с пробелами
            }

            string[] normalizedLines = emptyLinesTop.Concat(lines).Concat(emptyLinesBottom).ToArray();

            return string.Join("\n", normalizedLines);
        }
        public static void PrintCatArt(int index)
        {
            if (index < 0 || index >= catAsciiArt.Length)
            {
                Console.WriteLine("Invalid index");
                return;
            }

            string normalizedArt = NormalizeArt(catAsciiArt[index]);
            Console.WriteLine(normalizedArt);
        }

    }
}
