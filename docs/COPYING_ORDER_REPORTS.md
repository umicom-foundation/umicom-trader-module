# Copy an order report

An order report is a snapshot of the orders retained in your current Trader
workspace. It helps you review a paper session or compare results outside the
application. It is not a complete broker statement or reconciliation.

1. Open **Orders / Trade Blotter**.
2. Choose the order status and enter an order ID, instrument, symbol or venue.
   Select **Apply search**, or press Enter in the search box.
3. Check the matching and retained order counts.
4. Select **Copy applied orders CSV**. A message confirms that the report was
   offered to the clipboard.
5. Paste it into a text editor and save it as UTF-8 text with a `.csv` extension.
   To use a spreadsheet, import that file with comma separators and quoted
   fields. Direct pasting may not split CSV into columns in every spreadsheet.

The report includes the applied search, order coordinator revision, total retained
orders, matching orders and each matching order's account, instrument, status,
quantity, prices, filled quantity and order version. The environment column
identifies the environment recorded on each order. Unapplied search edits do
not change the export. An empty search result produces a header and summary
with no order rows.

The copy action does not send, cancel or amend an order, and does not change the
ticket. It reads the current applied query when clicked; later fills are not
added to an earlier copy. Execution-by-execution details remain in the order
review and are not separate rows in this report.

If text could be interpreted as a spreadsheet formula, Framework prefixes it
with an apostrophe. Import identifiers as text to preserve leading zeroes. Use
UTF-8 to preserve international names. Prices use a dot decimal separator and
may use scientific notation. If copying fails, the previous clipboard content
is left unchanged; review the message before pasting anything.
