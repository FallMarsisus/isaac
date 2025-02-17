from PIL import Image, ImageDraw, ImageFont
from wand.image import Image as WandImage
import os

fontName = "test_font"  # Ne pas mettre .ttf
font_size = 40
padding = 10

def text_to_bitmap(text, font_path, font_size, output_path_png, output_path_bmp):
    try:
        font = ImageFont.truetype(font_path, font_size)

        if not text.isprintable():
            print(f"Skipping non-printable character: {repr(text)}")
            return

        # Calculer la taille du texte
        bbox = font.getbbox(text)
        text_width = bbox[2] + padding
        text_height = bbox[3] + padding

        # Création de l'image avec un fond transparent
        image = Image.new('RGBA', (text_width, text_height), (255, 255, 255, 0))
        draw = ImageDraw.Draw(image)

        # Calculer les coordonnées pour centrer le texte
        x = (image.width - (bbox[2] - bbox[0])) // 2
        y = (image.height - (bbox[3] - bbox[1])) // 2

        # Dessin du texte
        draw.text((x, y), text, font=font, fill='black')

        # Enregistrer l'image avec un fond transparent
        image.save(output_path_png, format='PNG')

        # Convertir l'image PNG en BMP avec wand
        with WandImage(filename=output_path_png) as img:
            img.format = 'bmp'
            img.save(filename=output_path_bmp)
            os.remove(output_path_png)
            img.save(filename=output_path_bmp)

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
        output_path_png = os.path.join(output_dir, f"{i}.png")
        output_path_bmp = os.path.join(output_dir, f"{i}.bmp")
        text_to_bitmap(text, font_path, font_size, output_path_png, output_path_bmp)

saveASCII_Table(fontName)
