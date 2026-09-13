# uLoader (maintained fork)

Fork mantenido de **uLoader 5.1E**, el cargador de backups de Wii más completo de su época (Hermes). El proyecto original no se mantiene desde **2012**, así que este fork moderniza, corrige y documenta el código, y compila con **GitHub Actions** (devkitPro).

## Qué es uLoader

Loader de backups para Wii que soporta:

- **USB**: discos USB (WBFS / FAT32) con juegos de Wii.
- **DVD externo por USB** ("Modo DVD USB"): lee backups `.iso` desde un lector DVD externo conectado a un puerto USB (`saltar el botón 2` o el icono DVD).
- **DVD interno**: backups desde la propia lectora de la Wii (presionando `2`).
- **SD card** con FAT32.
- **Multijuegos en DVD** (hasta 8 juegos en un DVD UDF con `.ciso`).
- **cIOS propias**: requiere uno de los cIOS 222/223/224 (Hermes) o 249.
- Actualización de uLoader por Internet, códigos de trucos (Ocarina), memcard 2 compatible, NAND emulation (parcial), sonido/gráficos custom.

> El descubrimiento de juegos, los `.ciso` y el resto del sistema funcionan igual que el 5.1E original. Este fork NO cambia el comportamiento por defecto: solo corrige fallos, documenta y permite que se vuelva a construir.

## Mejoras de este fork

- **Fix: softlock del error "USB Device is detected as HUB!!!"** (`-20000`).
  En el original, al conectar un lector cuyo dispositivo se detecta como **hub USB** (por ejemplo muchos lectores externos de DVD con ranura SD/lector de tarjetas integrado), uLoader entraba en un **bucle infinito** del que solo se salía apagando la consola.
  Ahora:
  - Se muestra un mensaje claro (lectores con ranura SD integrada llevan un hub interno y no son compatibles).
  - Tras **5 s** (o inmediato si activas el nuevo hack) hace *autofallback* al modo DVD interno o a los juegos FAT (igual que el error `-100`).
  - Se puede salir al instante con el **botón 2** igual que en el resto de errores.
- **Nuevo hack en "uLoader Hacks": "Auto-Fallback on HUB"** (sustituye al hack `???` que no estaba implementado). Activa el fallback automático inmediato cuando se detecta un hub.
- **Fix: también se puede salir con el botón 2** de los errores de sector size (`-20001`/`-20002`), que antes quedaban en bucle infinito.
- **Build modernizado**: compilación reproducible con devkitPro + GitHub Actions (artifacts listos para copiar a la SD).
- Documentación actualizada en español e inglés.

> Nota técnica: leer el DVD que cuelga **detrás** de un hub USB requiere implementar *hub traversal* en el módulo EHCI del cIOS (código ARM del Starlet). Nadie lo ha implementado en la escena; este fork al menos evita el bloqueo y da salida limpia.

## Cómo se construye

Con **devkitPro** (devkitPPC + devkitARM) y GNU make:

```sh
# La app (boot.dol) necesita solo devkitPPC
cd src/uloader
make            # genera uloader.dol y uloader_alt.dol

# Opcional: reconstruir el módulo FAT del cIOS (devkitARM)
cd src/uloader/fatffs-module
make            # regenera resources_modules/fatffs-module.elf
```

En GitHub Actions esto ya está configurado (`.github/workflows/build.yml`): cada *push* a `main` compila los `.dol` y los deja como artifact descargable.

### Estructura rápida

```
src/uloader/          # la app (PowerPC, devkitPPC)
  uloader/            # build principal
  uloader_alt/        # build alternativa (-DALTERNATIVE_VERSION)
  source/             # código de la app
  resources_modules/  # módulos precargados (ehcmodule, dip_plugin, fatffs-module)
  fatffs-module/      # fuente del módulo FAT del cIOS (devkitARM)
src/screenlib/        # biblioteca de pantalla
src/isotociso/        # herramienta de conversión a .ciso (PC)
src/wbfs/             # utilidades wbfs
apps/                 # ejemplo de layout de SD
```

## Instalación en la SD

`apps/uloader/boot.dol` (y `boot.dol` de `uloader_alt`) van en:

```
sd:/apps/uloader/boot.dol
```

Requiere un cIOS compatible: **Hermes cIOS 222/223/224 (v5.1)** o un cIOS 249 (Waninkoko/d2x). El propio uLoader fuerza el cIOS 222/224 y tiene hacks para elegir.

## Créditos

- **Hermes** — autor original de uLoader y de los cIOS 222/223/224.
- **Rodries** — autor de uLoader 5.x junto a Hermes.
- Proyectos de base: Kwiirk (YAL), Waninkoko (USB Loader), WiiPower, oggzee…
- Este fork: mantenido por la comunidad, sin ánimo de lucro.

## Licencia

GPL-2.0 (ver `gpl-2.0.txt`).