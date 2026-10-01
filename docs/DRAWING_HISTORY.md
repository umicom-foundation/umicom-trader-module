# Correct a drawing with undo and redo

Use drawing history when you want to reverse a completed chart edit. It includes
drawing creation, movement, duplication, removal, locks, visibility and appearance.
Your orders and order-entry draft stay separate from these actions.

1. Select the instrument whose chart you want to edit.
2. Draw a trend line, support level, resistance level, range, ray or liquidity box.
3. Choose **Undo drawing** to reverse the latest drawing step. Read the label
   beside the controls to see the operation and instrument it belongs to.
4. Choose **Redo drawing** to reapply that step.
5. To test appearance history, select a drawing and choose **Load selected** under
   **Drawing appearance**. Change its colour, apply it, and undo the change.
6. Choose **Save chart** when you want to preserve the displayed chart state.

Click the chart canvas before using Ctrl+Z, Ctrl+Shift+Z or Ctrl+Y. The first
shortcut undoes; the other two redo. An unfinished drawing or move is cancelled
after a successful history action. Escape cancels an unfinished gesture directly.

History follows the order of edits across the workspace. If the next step belongs
to another instrument, select that instrument. The control does not silently
switch charts or change a drawing elsewhere. A new drawing edit after undo
replaces the redo branch; setting a value that is already present leaves it alone.

Saving does not clear history. Restoring a saved chart does clear it because the
restore replaces both drawings and their saved view. History is session memory:
closing the workspace loses it. Up to 64 steps fit in an 8 MiB retained-history
budget; large edits may make older steps unavailable sooner.

If a button is unavailable, read the history label. There may be no step, it may
belong to another instrument, or another component may have changed the drawing
registry. In the last case, the next successful drawing edit starts a new history.
If an action reports that history changed, inspect the refreshed chart before
trying again. If appearance fields still show an older draft, choose **Load
selected** again before applying it to the restored drawing.

Undo does not change zoom, timeframe, studies, prices, orders, fills or broker
connections. It cannot cancel an order. See [drawing appearance](DRAWING_APPEARANCE.md)
for colours, widths and fill settings.

For product integrations, `UmiTraderGtkDrawingHistory` returns a copied snapshot.
Pass its displayed revision and selected instrument to `UmiTraderGtkUndoDrawing`
or `UmiTraderGtkRedoDrawing`. These calls use the same Framework history as the
native controls and reject delayed actions whose evidence has changed.
