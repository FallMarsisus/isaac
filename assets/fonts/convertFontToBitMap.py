from PIL import Image, ImageDraw, ImageFont
from wand.image import Image as WandImage
import os

fontName = "test_font"  # Ne pas mettre .ttf
font_size = 40
resolution_scale = 4  # Facteur de multiplication pour la résolution

# Définition des paddings d'origine
left_padding = 2
right_padding = 3    # 1 pixel de plus à droite
top_padding = 5
bottom_padding = 7   # 2 pixels de plus en bas

# Application du facteur de résolution
scaled_left_padding = left_padding * resolution_scale
scaled_right_padding = right_padding * resolution_scale
scaled_top_padding = top_padding * resolution_scale
scaled_bottom_padding = bottom_padding * resolution_scale

def compute_common_metrics(font):
    max_ascent = 0
    max_descent = 0
    for i in range(256):
        char = chr(i)
        if not char.isprintable():
            continue
        mask = font.getmask(char)
        bbox = mask.getbbox()
        if bbox:
            ascent = -bbox[1] if bbox[1] < 0 else 0
            descent = bbox[3]
            if ascent > max_ascent:
                max_ascent = ascent
            if descent > max_descent:
                max_descent = descent
    return max_ascent, max_descent

def text_to_bitmap(text, font, common_height, common_baseline, output_path_png, output_path_bmp):
    try:
        if not text.isprintable():
            print(f"Skipping non-printable character: {repr(text)}")
            return

        mask = font.getmask(text)
        bbox = mask.getbbox()
        if bbox is None:
            return

        # Calcul de la largeur de l'image : largeur du glyph + paddings gauche et droite
        text_width = (bbox[2] - bbox[0]) + scaled_left_padding + scaled_right_padding

        image = Image.new('RGBA', (text_width, common_height), (255, 255, 255, 0))
        draw = ImageDraw.Draw(image)

        # Positionnement : 
        # x positionne le glyph pour commencer à scaled_left_padding
        x = scaled_left_padding - bbox[0]
        # y ajuste le dessin pour que la baseline soit identique pour tous
        y = common_baseline + bbox[1]

        draw.text((x, y), text, font=font, fill='white')

        image.save(output_path_png, format='PNG')

        with WandImage(filename=output_path_png) as img:
            img.format = 'bmp'
            img.save(filename=output_path_bmp)
            os.remove(output_path_png)

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

    # Charger la police à la taille multipliée pour une résolution plus élevée
    scaled_font_size = font_size * resolution_scale
    font = ImageFont.truetype(font_path, scaled_font_size)

    max_ascent, max_descent = compute_common_metrics(font)
    common_height = max_ascent + max_descent + scaled_top_padding + scaled_bottom_padding
    common_baseline = scaled_top_padding + max_ascent

    for i in range(256):
        text = chr(i)
        output_path_png = os.path.join(output_dir, f"{i}.png")
        output_path_bmp = os.path.join(output_dir, f"{i}.bmp")
        text_to_bitmap(text, font, common_height, common_baseline, output_path_png, output_path_bmp)

saveASCII_Table(fontName)
