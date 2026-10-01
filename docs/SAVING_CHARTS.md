# Save and restore your chart drawings

A chart checkpoint is a saved copy of one instrument's drawings and view.
It keeps trend lines, support and resistance levels, drawing styles, selected
and locked flags, the zoom level and the historical time anchor. Each instrument
has its own saved copy in your local profile.

## Save a chart

1. Open Trader and sign in to your local profile.
2. Select an instrument in the watchlist.
3. Add drawings with **Trend**, **Support** or **Resistance**. Use the mouse
   wheel or **+** and **-** to zoom; drag the chart to move through retained bars.
4. Choose **Save chart** below the drawing list.
5. Read the message. A successful disk save says that drawings and the view
   were saved to this profile. A memory-only save will not survive closing
   the application.

Saving a layout does not save chart drawings. Chart saves are explicit: changing
an instrument, closing a panel or receiving a price update does not save a chart.

## Open a saved chart again

1. Open the same local profile and select the same instrument.
2. Choose **Preview saved chart**.
3. Expand **Saved chart details**. Check the instrument, drawing count, view
   settings and each drawing's coordinates, style and lock flag.
4. Choose **Restore preview** only when you want those saved drawings and that
   view to replace the current chart for this instrument.

Restore replaces the whole drawing set for this instrument, including locked
drawings. A saved empty chart therefore clears its current drawings. Other
instruments keep their drawings. Prices, order tickets and orders are unchanged.
Drawings can be outside the current price history; saving them does not download
the historical bars needed to display that period.

If you edit the chart after previewing, another panel creates a newer preview,
or another window changes the saved copy, restore can be refused. Preview again
and review the latest state. A preview does not reserve or lock the saved chart.

## Replace a saved copy

After restarting Trader, a saved chart may already exist. The first Save request
will ask you to preview it. Choose **Preview saved chart**, review its details,
then choose **Save chart** to replace it with the current chart. Choose
**Restore preview** instead if you want to bring the saved chart into the workspace.

Two windows cannot silently overwrite each other's saved revisions. If a save
is refused because the stored chart changed, preview it again before deciding
which version to keep. Your current chart stays available after a failed save.

## Recovery and storage

The previous valid save is kept alongside the current one. If the current copy
is missing or damaged, Preview may offer the previous valid copy. Its message
will identify the recovery. Review it before restoring.

Recovery does not repair or overwrite damaged storage. Further saves to that
damaged chart remain refused. Keep the application open if the restored chart
contains work you need, preserve a copy of the profile database, and report the
storage error before attempting repair. There is no chart-file export or repair
button in this version.

The normal launcher uses the existing profile directory under your operating
system's user configuration folder. Chart records share `workspace-layouts.sqlite3`
with layout records, using a separate record namespace and database connection.
The location does not depend on the folder from which Trader was launched.
Local profiles separate stored work; they are not encryption or broker accounts.

Checkpoints do not contain candle history, moving-average settings, alerts,
broker connections, orders or credentials. Those features have their own owners.
Chart orders still prepare an order ticket for review; saving or restoring a
chart never submits an order.

## Additional drawing tools

Range, Liquidity zone and Ray now use the same saved-chart workflow as Trend,
Support and Resistance. You may use any of the six tools in step 3 above.
The checkpoint also retains edits made with Move selected, Lock / Unlock and
Duplicate. [Mark and edit chart objects](CHART_DRAWING_OBJECTS.md) shows these
controls and explains how a reviewed restore treats locked drawings.

## Save hidden drawings

Hide / Show, Hide all and Show all change the current chart. Use **Save chart**
after these actions to keep their results. A hidden drawing remains in the
saved set with its geometry and lock. **Saved chart details** identifies each
drawing as hidden or visible, so you can review those choices before restoring.

Changing visibility after a preview invalidates that preview. Choose
**Preview saved chart** again before deciding to restore. A saved empty chart
and a saved chart containing hidden drawings are different: the first has no
drawings, while the second can be revealed with **Show all** after restore.

Saved charts from before visibility support load with their drawings visible.
After saving with this version, older versions cannot read its new drawing
records and may offer an older recovery copy. Keep a backup of your profile
database before returning to an older application.


## Keep per-drawing appearance

After using **Apply appearance** or **Use tool defaults**, choose **Save chart**.
The same checkpoint keeps colour, width and box fill with the drawing's
coordinates. Restoring a reviewed checkpoint restores those choices too.
[Change drawing appearance](DRAWING_APPEARANCE.md) explains the editor and
how it handles older saved styles.

## Keep the selected timeframe

Save chart also keeps the interval selected in Timeframe. Saved chart details
shows both saved and current choices before restore. Read
[change the chart timeframe](CHART_TIMEFRAMES.md) for steps, older-save
compatibility and the limits of retained price observations.
