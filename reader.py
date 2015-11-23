
import bs4
f=file('scrabble.html')
lngstr=f.read()
f.close()
soup = bs4.BeautifulSoup(lngstr)

def tileData(cell):
    if cell.findChild(attrs={'class':'tile'}):
        return cell.findChild(attrs={'class':'tile'}).attrs['data-letter']
    if 'tripleWord' in cell.attrs['class']:
        return u'9'
    if 'doubleWord' in cell.attrs['class']:
        return u'4'
    if 'tripleLetter' in cell.attrs['class']:
        return u'3'
    if 'doubleLetter' in cell.attrs['class']:
        return u'2'
    if 'startingPoint' in cell.attrs['class']:
        return u'4'
    return u'-'

f=file('board.txt','w')

rackCells = soup.findChild(attrs={'id':'tileRackContainer'}).findChildren(attrs={'class':'tile'})
for tile in rackCells:
    value = tile.attrs['data-letter']
    if value == "BLANK":
      f.write("*")
    else:
      f.write(value)
f.write("\n")

i=0
for cell in soup.findAll(attrs={'class':'boardCell'}):
    f.write(tileData(cell))
    i += 1
    if i == 15:
      f.write('\n')
      i = 0
f.close()