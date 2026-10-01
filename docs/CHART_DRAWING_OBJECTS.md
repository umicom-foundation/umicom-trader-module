# Mark a chart with ranges, zones and rays

Trader's chart panel offers Trend, Support and Resistance alongside Range,
Liquidity zone and Ray. Drawings belong to the selected instrument. They are
your annotations; they do not change market prices or submit orders.

## Draw a range or a zone

1. Select an instrument with retained price bars in the watchlist.
2. Choose **Range** for an outline, or **Liquidity zone** for a shaded box.
3. Click one corner in the price chart, then click the opposite corner.
4. Use different times and prices for the two corners. The corners may be chosen
   in either order. If no box appears, read the message below the controls.
5. Choose **Cursor** to return to panning. Use the mouse wheel or **+** and **-**
   to zoom, or **Fit / Latest** to return to the full retained view.

A liquidity zone marks an area you want to study. Its shading does not claim
that a live order book or a broker has measured liquidity there.

## Draw a ray

1. Choose **Ray**.
2. Click the origin of the ray.
3. Click a second point at a different time to set its direction and slope.

The ray continues from the first point through the second. A second point to
the left makes a leftward ray. Only the part inside the visible price chart is
shown. Panning and zooming change the view, not the stored anchors.

## Move, lock or copy a drawing

1. Choose the drawing in the object list below the chart. Each entry includes
   its tool, first price and ID; locked entries also say **(locked)**.
2. Choose **Move selected** and click replacement anchors. A support or
   resistance level needs one click; a line, ray or box needs two.
3. Press **Escape** while the chart has focus to cancel a move. Choosing another
   object or changing instrument also cancels it. A finished or cancelled move
   returns to **Cursor**. Select a drawing tool explicitly to create another.
4. Choose **Lock / Unlock** to protect a drawing from movement and individual
   removal. Choose it again to allow editing.
5. Choose **Duplicate selected** to make an unlocked copy. The copy is selected in the
   list and initially overlaps its source. Choose **Move selected** to place it.

If another chart view changes the record during a move, Trader refuses that
move. Select the current object again and review it before retrying. A refused
move does not create a replacement drawing or change the original anchors.

The tool buttons create drawings repeatedly until you choose another tool.
During creation, Escape cancels the unfinished anchors while keeping that tool
available. Moving an existing drawing instead returns to Cursor when finished.

## Keep your work

Choose **Save chart** after drawing or editing. Follow
[Save and restore your chart drawings](SAVING_CHARTS.md) to preview and restore
a saved copy. The checkpoint keeps ranges, zones and rays alongside the older
tools, including their anchors and locks. Saving a layout alone does not save
chart drawings.

Review a saved chart before restoring it: restoring replaces this instrument's
whole drawing set, including locked drawings. Keep a copy of any current work
you need before choosing that replacement. A save to memory does not survive
closing the application; the save message distinguishes memory from disk.

## Current limits

<!-- Hide/show now uses Framework's canonical visibility and checkpoint state.
The previous capability description is retained for engineering review.
Move uses replacement anchors rather than draggable handles. There is no drawing
undo, hide/show, grouping, drawing alert or per-object colour editor yet.
-->
<!-- The shared appearance editor replaces the former colour-editor limitation.
The earlier description is retained for engineering review.
Move uses replacement anchors rather than draggable handles. There is no drawing
undo, named grouping, drawing alert or per-object colour editor yet.
-->
Move uses replacement anchors rather than draggable handles. There is no drawing
undo, named grouping or drawing alert yet. The [appearance editor](DRAWING_APPEARANCE.md)
changes each drawing's colour, width and box fill.
Drawings outside the retained time or price range may be invisible until the
relevant history is available. Drawing a ray does not request more market data.

The separate Buy limit and Sell limit tools prepare an order ticket for review.
Drawing, moving, locking, duplicating, saving and restoring annotations do not
submit that ticket or arm live trading.

## Hide drawings without losing them

1. Choose a drawing in the object list and select **Hide / Show**.
2. Its annotation disappears from the chart, but the entry remains with
   **(hidden)**. The coordinates, style and lock are retained.
3. Choose **Hide / Show** again to show it. Show a hidden drawing before moving it.
4. Choose **Hide all** or **Show all** to change visibility for this instrument.
   These buttons leave other instruments and order markers alone.
5. Choose **Save chart** to keep the choices between sessions.

You can hide a locked drawing. A duplicate inherits its source's visibility,
so a duplicate of a hidden drawing is initially hidden too. **Show all** shows
all drawings; it does not restore the mix of visible and hidden drawings that
existed before **Hide all**.

If an action reports that the drawing or list changed, review the current
entries before trying again. A visibility action cancels an unfinished move.
Hidden drawings still occupy the drawing list and count towards its capacity.
