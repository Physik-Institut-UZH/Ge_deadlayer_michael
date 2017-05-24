echo "Installing the paViewer in ~/bin"
echo "This should be in your \$PATH !"
echo "In order to use the paViewer the "
echo "following packages must be installed:"
echo "- python"
echo "- gtk / pygtk"
echo "- matplotlib"

cp -v paViewer.py ~/bin
cp -v paViewerGUI.glade ~/bin
cp -v PA.py ~/bin
cp -v AGATA-logo.png ~/bin


