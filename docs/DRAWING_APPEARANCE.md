# Change the appearance of a chart drawing

You can give each trend line, support level, resistance level, ray, range or
liquidity zone its own colour and outline width. Ranges and liquidity zones
also support a translucent fill. These choices describe your chart annotations.
They do not alter market prices, change the order ticket or place an order.

## Choose an appearance

1. Create a drawing, or restore a saved chart using **Preview saved chart** and
   **Restore preview**.
2. Select its entry in **Drawings** and expand **Drawing appearance**.
3. Choose **Load selected**. The explanation identifies the drawing whose
   fields you are editing. Loading does not change the drawing.
4. Enter a colour such as **#33AAFF**. The six hexadecimal digits describe
   red, green and blue, in that order; each pair ranges from **00** to **FF**.
   For example, **#FF0000** is red and **#FFFFFF** is white.
5. Set **Width** between **1.0** and **8.0** in steps of **0.1**. This measures
   logical chart pixels; the renderer scales the chart with its window.
6. For a range or liquidity zone, set **Fill %** between **0** and **80**.
   Zero removes the fill; a larger number makes it less transparent. Lines,
   rays and support/resistance levels do not have a fill.
7. Choose **Apply appearance**. The chart redraws with the chosen outline
   colour and width. Support and resistance labels use the outline colour.
8. Choose **Save chart** to keep the change with this instrument's saved
   drawings. Check whether the save message reports disk or memory storage.
   A save to memory lasts only for the current application session.

Typing in the fields alone does not change or save anything. Price updates
leave your draft fields intact. **Load selected** replaces those fields with
the current drawing's appearance; use it to discard an unfinished edit.

## Return to the tool's appearance

Load the current drawing, then choose **Use tool defaults**. This removes its
custom appearance. Support and resistance use their theme colours; other
tools use their standard colours and widths. A default liquidity zone has a
light blue fill; a default range has only an outline. Save the chart afterwards
if you want to keep the reset.

The colour fields display an RGB approximation of theme defaults. Applying
those fields creates a fixed custom colour. **Use tool defaults** keeps the
drawing linked to its tool's defaults instead.

## If the drawing changes while you are editing

An edit is tied to the drawing and version you loaded. Switching the selected
drawing or instrument, moving the drawing, changing its lock or visibility,
or restoring a chart can make that edit stale. Apply then leaves the model
unchanged and asks you to load again.

Your fields remain available for inspection. Select the intended drawing,
choose **Load selected**, review its latest state, then enter and apply the
new values. Loading replaces the unfinished fields.

An older or unrecognised saved style displays a notice. Merely loading it
keeps its stored text. **Apply appearance** explicitly replaces it with your
choices, and **Use tool defaults** explicitly clears it. Leave both actions
unused if you want to retain the original saved style.

## Locks, copies and saved charts

The drawing lock protects movement and removal. You can still change colour,
width, fill or visibility while a drawing is locked. Editing a hidden drawing
does not show it; use **Hide / Show** when you want to see it again.

**Duplicate selected** copies the appearance as well as the geometry and
visibility. The duplicate remains independently editable. Saved charts include
the appearance in the same reviewed save/restore workflow as the coordinates.

There is no appearance undo history, dash-pattern editor, gradient or bulk
appearance command. Preview a saved chart before restoring: a restore replaces
the instrument's whole drawing set and view, including changes to other drawings.

See [drawing tools](CHART_DRAWING_OBJECTS.md) and
[saving chart drawings](SAVING_CHARTS.md) for the related workflows.

[Correct completed edits with drawing history](DRAWING_HISTORY.md) explains how to reverse an applied appearance and when to load the drawing again.
