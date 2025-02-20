import os
from fontTools.ttLib import TTFont
from fontTools.pens.svgPathPen import SVGPathPen
from fontTools.pens.boundsPen import BoundsPen

def glyph_to_svg_path_and_bounds(font, char):
    """
    Pour un caractère donné, retourne le tracé SVG et sa boîte englobante.
    """
    cmap = font.getBestCmap()
    codepoint = ord(char)
    glyph_name = cmap.get(codepoint)
    if glyph_name is None:
        return None, None
    glyphSet = font.getGlyphSet()
    glyph = glyphSet[glyph_name]
    
    # Extraction du tracé sous forme de commande SVG
    pen = SVGPathPen(glyphSet)
    glyph.draw(pen)
    path_data = pen.getCommands()  # utilisation de getCommands() à la place de getSVGPath()
    
    # Calcul de la boîte englobante
    bounds_pen = BoundsPen(glyphSet)
    glyph.draw(bounds_pen)
    bounds = bounds_pen.bounds  # (xmin, ymin, xmax, ymax)
    return path_data, bounds

def save_glyph_svg(font_path, output_dir, target_height=40, fill_color="#030104"):
    """
    Pour chaque caractère imprimable (ASCII 0 à 255) de la police,
    extrait le tracé vectoriel et génère un fichier SVG dans output_dir.
    
    Le glyphe est mis à l'échelle pour que sa hauteur corresponde à target_height pixels.
    La transformation applique également l'inversion de l'axe Y pour le système SVG.
    """
    font = TTFont(font_path)
    os.makedirs(output_dir, exist_ok=True)
    
    for i in range(256):
        char = chr(i)
        if not char.isprintable():
            print(f"Caractère non imprimable ignoré : {repr(char)}")
            continue
        
        path_data, bounds = glyph_to_svg_path_and_bounds(font, char)
        if path_data is None or bounds is None:
            print(f"Aucun glyphe ou boîte pour : {repr(char)}")
            continue
        
        xmin, ymin, xmax, ymax = bounds
        glyph_height = ymax - ymin
        if glyph_height == 0:
            print(f"Glyph vide pour {repr(char)}")
            continue
        
        # Calculer le facteur d'échelle pour que la hauteur du glyphe devienne target_height pixels
        scale = target_height / glyph_height
        target_width = (xmax - xmin) * scale
        
        # Transformation :
        #   - Échelle par "scale"
        #   - Inversion verticale (y négatif) pour passer du repère de la police au repère SVG
        #   - Translation pour placer le glyphe en (0,0)
        transform = f"matrix({scale} 0 0 {-scale} {-xmin*scale} {ymax*scale})"
        viewBox = f"0 0 {target_width} {target_height}"
        
        svg_content = f'''<svg xmlns="http://www.w3.org/2000/svg" viewBox="{viewBox}" height="{target_height}px" width="{target_width}px">
  <g transform="{transform}">
    <path d="{path_data}" fill="{fill_color}" />
  </g>
</svg>'''
        
        output_file = os.path.join(output_dir, f"{i}.svg")
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(svg_content)
        print(f"SVG sauvegardé pour {repr(char)} (code {i}) dans {output_file}")


if __name__ == "__main__":
    font = "calibri"
    
    font_size = 50
    
    
    filePath = os.path.dirname(__file__)
    fontPath = os.path.join(filePath, f"{font}.ttf")
    output_dir = os.path.join(filePath, f"{font}SVG")
    os.makedirs(output_dir, exist_ok=True)

    
    if not os.path.exists(fontPath):
        print(f"Police non trouvée : {fontPath}")
    else:
        save_glyph_svg(fontPath, output_dir, fill_color="#FFFFFF", target_height=font_size)