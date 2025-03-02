#include "leetcode.h"

char* convert(char* s, int numRows) 
{
	const int STRING_LENGTH = strlen(s);
	char* zigzag = malloc((STRING_LENGTH + 1) * sizeof(char));
	zigzag[STRING_LENGTH] = '\0';

	int zigzagIdx = 0;
	for (int rowIdx = 1; rowIdx <= numRows; rowIdx++)
	{

		int rowOffset = 0;
		bool useFirstFormula = true;
		for (int charIdxForRow = rowIdx - 1; charIdxForRow < STRING_LENGTH; charIdxForRow += rowOffset)
		{
			if (rowIdx == 1 || rowIdx == numRows) rowOffset = 2 * (numRows - 1) > 0 ? 2 * (numRows - 1) : 1;
			else
			{
				rowOffset = useFirstFormula ? 2 * (numRows - rowIdx) : 2 * (rowIdx - 1);
				useFirstFormula = !useFirstFormula;
			}

			zigzag[zigzagIdx] = s[charIdxForRow];
			zigzagIdx++;
		}
	}

	return zigzag;
}

