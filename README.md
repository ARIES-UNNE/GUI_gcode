# Generador de G-Codes

Este proyecto es una aplicación desarrollada con Qt y Python que permite generar códigos G (G-codes) para aplicaciones de bioimpresión en el control de máquinas CNC.
Utiliza principalmente las librerias de "QOpenGL" y "QWidget" además de "Math" para el archivo Python.

## Características

- Interfaz de usuario desarrollada con Qt.
- Generación de G-CODES personalizados.
- Visualización del G-CODE en tiempo real.
- Soporte para configuraciones de guardado.
- Soporte para varios idiomas y temas.
- Compatible con Windows.

![image](https://github.com/user-attachments/assets/661bfd86-a36b-47c9-a3d0-bf20a6b41690)


- La generación de los G-CODES se realiza mediante el siguiente Script de Python: https://github.com/ARIES-UNNE/GUI_gcode/blob/main/axolotl_2mat.py

- Ejemplo del G-CODE que se genera a partir del generador: https://github.com/ARIES-UNNE/GUI_gcode/blob/main/Axo3_1mat.gcode

- El reporsitorio cuenta con versiones anteriores del Genreador de G-CODES como: https://github.com/ARIES-UNNE/GUI_gcode/blob/main/axolotl_1mat.py 

- Se puede consultar la documentacón doxygen dentro de la release


## Requisitos

No es necesario descargar programas o librerías externas.

## Instrucciones de Instalación

- Para ejecutar la aplicación, descarga el archivo release y ejecuta el `.exe` de la aplicación.
- Release: [https://github.com/ARIES-UNNE/GUI_gcode/releases](https://github.com/ARIES-UNNE/GUI_gcode/releases)

## Clonar el repositorio

git clone https://github.com/ARIES-UNNE/GUI_gcode.git


## Uso:

1. Ejecutar la aplicación:
   - Abre la aplicación ejecutando el archivo .exe descargado desde el release.

![image](https://github.com/user-attachments/assets/6bd80083-41db-45e6-ab6e-ea32c1004b21)

   
2. Configurar los parámetros:
   - En la interfaz, ingresa los parámetros necesarios para la generación del G-CODE deseado. 

3. Generar el código G:
   - Una vez configurados los parámetros, haz clic en el botón "SAVE GCODE" para obtener el código G. Este se generará basado en las configuraciones que hayas especificado.

4. Guardar configuración de guardado:
   - Puedes guardar los parámetros establecidos mediante el botón "CREATE SAVE CONFIG", estos se pueden cargar nuevamente en la aplicación, en la primera ventana.
   

![image](https://github.com/user-attachments/assets/f31ce658-f437-4cad-8603-a1000d499172)



## Créditos

- Desarrollador Principal: Carlos Barquín Mazuela (https://github.com/CarlosBarquin)
- Colaboradores: Nieves Cubo Mateo (https://github.com/Nicuma3)
  
Este proyecto se desarrolló como parte del Grupo de Investigación ARIES.


## Licencia

Este proyecto está bajo la licencia MIT. Consulta el archivo LICENSE para más detalles.

