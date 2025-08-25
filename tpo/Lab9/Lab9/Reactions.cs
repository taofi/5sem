using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab9
{
    class Reactions : IReactions
    {
        public string PlayReaction(int food)
        {
            Random random = new Random();
            int reactionIndex = random.Next(CatStrings.PositiveReactions.Length);
            return food > 60 ? CatStrings.PositiveReactions[reactionIndex] : CatStrings.NegativeReactions[reactionIndex];
        }
    }
}
