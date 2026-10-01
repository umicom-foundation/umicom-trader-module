# Change the chart timeframe

The Timeframe selector lets you view retained prices as Source bars, 1-minute,
5-minute, 15-minute, 1-hour, 4-hour or daily UTC candles.

1. Open the **Chart** tool and choose an instrument with price history.
2. Choose **5m** beside **Timeframe**. Look at the status: it shows the interval,
   how many candles are visible, and how many source bars produced them.
3. Zoom or move earlier/later to inspect those candles. **Fit** shows all
   retained candles in that same timeframe.
4. Add a moving average if needed. Its period counts candles in this timeframe.
   A 20-period SMA needs at least 20 retained candles before it can appear.
5. Switch back to **Source** to see the provider's original records.

Drawings keep their original time and price coordinates, so their horizontal
position can change when the displayed time range changes. Changing timeframe
does not place an order or modify the order draft. Each instrument keeps its
own interval while the workspace remains open.

## Save your choice

Choose **Save chart** after selecting your preferred interval and position.
When returning to a saved view, choose **Preview saved chart**, compare the
saved and current timeframes in **Saved chart details**, then use
**Restore preview**. A changed interval invalidates an older preview; preview
again before restoring. [Saving charts](SAVING_CHARTS.md) explains the storage
and recovery messages.

An older saved chart opens in Source mode. New saves include the selected
timeframe. Older versions of Trader cannot read the new view metadata and
may offer an older recovery copy. Back up your profile database before
returning to an older application version.

## When the result is different from a market data website

The chart groups only observations already available in this workspace. It
does not fetch extra history or fill missing bars. The first and last candles
can represent only part of an interval, and simulator event bars do not promise
complete minute histories. Daily means midnight to midnight UTC, not an
exchange session. Weekly, monthly and custom intervals are not available.

If a source bar crosses a selected interval's boundary, the chart cannot
reconstruct smaller candles from it. The selector keeps the previous interval
and explains the problem. Try Source or a compatible larger interval.
If a restored interval cannot display newly received data, select Source.
Invalid provider data must be corrected at its source; changing the view does
not repair it. Missing periods stay missing, and volume totals describe only
the retained observations.
