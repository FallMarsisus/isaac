from PIL import Image, ImageDraw, ImageFont
import os

fontName = "calibri"
font_size = 40
padding = 0

def text_to_bitmap(text, font_path, font_size, output_path):
    try:
        font = ImageFont.truetype(font_path, font_size)

        if not text.isprintable():
            print(f"Skipping non-printable character: {repr(text)}")
            return
        
        # Bounding box réelle du caractère
        mask = font.getmask(text)
        bbox = mask.getbbox()

        if not bbox:
            print(f"Skipping empty character: {repr(text)}")
            return

        width, height = bbox[2] - bbox[0], bbox[3] - bbox[1]

        # Hauteur standardisée pour éviter le décalage
        ascent, descent = font.getmetrics()
        total_height = ascent + descent + 2 # Hauteur totale de la ligne standard

        # Création de l'image avec padding
        image = Image.new('RGBA', (width + padding, total_height + padding), (255, 255, 255, 0))
        draw = ImageDraw.Draw(image)

        # Calcul du décalage vertical
        y_offset = ascent - bbox[3]  # Ajustement par rapport à la ligne de base

        # Dessin du texte
        draw.text((-bbox[0] + padding // 2, y_offset + padding // 2), text, font=font, fill=(0, 0, 0, 255))

        # Crop 5 px en haut et en bas
        image = image.crop((0, 5, image.width, image.height - 5))

        image.save(output_path)
    except Exception as e:
        print(f"Error processing {repr(text)}: {e}")

def saveASCII_Table(fontName):
    filePath = os.path.dirname(__file__)
    font_path = os.path.join(filePath, f"./{fontName}.ttf")

    if not os.path.exists(font_path):
        print(f"Font {font_path} not found!")
        return

    output_dir = os.path.join(filePath, fontName)
    os.makedirs(output_dir, exist_ok=True)

    for i in range(256):
        text = chr(i)
        output_path = os.path.join(output_dir, f"{i}.png")
        text_to_bitmap(text, font_path, font_size, output_path)

saveASCII_Table(fontName)
