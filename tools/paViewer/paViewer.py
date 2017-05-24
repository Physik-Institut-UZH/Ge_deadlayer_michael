#!/usr/bin/env python
import sys
import os
import threading
import PA
import matplotlib
from matplotlib.figure import Figure
from matplotlib.backends.backend_gtkagg import FigureCanvasGTKAgg
from matplotlib.pyplot import colorbar
#matplotlib.use('GTK') 
import numpy
from numpy.random import randn


try: 
	import pygtk 
	pygtk.require("2.0") 
except: 
    pass 
try: 
	import gtk 
	import gtk.glade 
except: 
	sys.exit(1)

gtk.gdk.threads_init()

class drawFigureThreat(threading.Thread):
	def run(self):
		
		global hwg
		hwg.draw_lock = hwg.draw_lock + 1
		if hwg.pa == None:
			return 0;				
		data_raw = []
		data_geo = []
		data_pot = []
		if hwg.plane == "xy":
			i = 0
			while i < hwg.pa.nx():
				j = 0
				data_raw.append([])
				data_geo.append([])
				data_pot.append([])
				while j < hwg.pa.ny():
					data_pot[i].append(hwg.pa.potential(i,j,hwg.currentZ))
					data_geo[i].append(hwg.pa.electrode(i,j,hwg.currentZ))
					data_raw[i].append(hwg.pa.raw(i,j,hwg.currentZ))
					j = j + 1
				i = i + 1
		elif hwg.plane == "xz":
			i = 0
			while i < hwg.pa.nx():
				j = 0
				data_raw.append([])
				data_geo.append([])
				data_pot.append([])
				while j < hwg.pa.nz():
					data_pot[i].append(hwg.pa.potential(i,hwg.currentY,j))
					data_geo[i].append(hwg.pa.electrode(i,hwg.currentY,j))
					data_raw[i].append(hwg.pa.raw(i,hwg.currentY,j))
					j = j + 1
				i = i + 1
		elif hwg.plane == "yz":
			i = 0
			while i < hwg.pa.ny():
				j = 0
				data_raw.append([])
				data_geo.append([])
				data_pot.append([])
				while j < hwg.pa.nz():
					data_pot[i].append(hwg.pa.potential(hwg.currentX,i,j))				
					data_geo[i].append(hwg.pa.electrode(hwg.currentX,i,j))
					data_raw[i].append(hwg.pa.raw(hwg.currentX,i,j))
					j = j + 1
				i = i + 1
		gtk.gdk.threads_enter()
		hwg.figure.clear()

		hwg.ax1 = hwg.figure.add_subplot(131)
		hwg.ax1.set_xlabel('Y')
		hwg.ax1.set_ylabel('X')
		hwg.ax1.set_title('Raw')
		
		hwg.ax2 = hwg.figure.add_subplot(132)
		hwg.ax2.set_xlabel('Y')
		hwg.ax2.set_ylabel('X')
		hwg.ax2.set_title('Geo')

		hwg.ax3 = hwg.figure.add_subplot(133)
		hwg.ax3.set_xlabel('Y')
		hwg.ax3.set_ylabel('X')
		hwg.ax3.set_title('Pot')		

		hwg.ax1.imshow(data_raw)
		hwg.ax2.imshow(data_geo)
		hwg.ax3.imshow(data_pot)	
		hwg.canvas.draw()
		gtk.gdk.threads_leave()
		print hwg.draw_lock
		hwg.draw_lock = hwg.draw_lock - 1

class appGui:
	def __init__(self): 
		gladefile = "paViewerGUI.glade" 
		self.windowname = "MainWindow" 
		self.wTree = gtk.glade.XML(gladefile, self.windowname) 
		dic = {"on_MainWindow_destroy" : gtk.main_quit, "on_ButtonQuit_clicked" : gtk.main_quit, "on_ButtonOpen_clicked" : self.Open,
			"on_ButtonPlane_clicked" : self.ChangePlane, "on_change_Current" : self.ChangeCurrent,
			"on_ButtonSave_clicked" : self.SaveFigure}
		self.wTree.signal_autoconnect(dic)
		self.window = self.wTree.get_widget(self.windowname)

		self.XmaxIndicator = self.wTree.get_widget("LabelXMax")
		self.YmaxIndicator = self.wTree.get_widget("LabelYMax")
		self.ZmaxIndicator = self.wTree.get_widget("LabelZMax")

		self.XScaler = self.wTree.get_widget("ScaleX")
		self.YScaler = self.wTree.get_widget("ScaleY")
		self.ZScaler = self.wTree.get_widget("ScaleZ")

		self.XText = self.wTree.get_widget("EntryX")
		self.YText = self.wTree.get_widget("EntryY")
		self.ZText = self.wTree.get_widget("EntryZ")

		self.PlaneSwitcher = self.wTree.get_widget("ButtonPlane")
		self.PlaneIndicator = self.wTree.get_widget("LabelPlane")

		self.window.show()

		self.OpenFileName = os.getcwd()

		self.plane = "xy"
		self.lock = 0
		self.draw_lock = 0
		self.pa = None
		self.TitleOfPlots = []
		self.ax = []

		self.figure = Figure(figsize=(6,6), dpi=72)
		self.figure.subplots_adjust(left=0.05, right=1.0, bottom=0.07, top=0.95, wspace=0.2, hspace=0.1)

		self.canvas = FigureCanvasGTKAgg(self.figure) # a gtk.DrawingArea
		self.canvas.show()

		self.view = self.wTree.get_widget("ViewFrame")
		self.view.add(self.canvas)

	def DirectOpen(self, filename):
		self.OpenFileName = filename
		self.pa = PA.PA(file = self.OpenFileName)

		self.currentX = self.pa.nx()/2
		self.currentY = self.pa.ny()/2
		self.currentZ = self.pa.nz()/2

		self.XmaxIndicator.set_text(str(self.pa.nx()))
		self.YmaxIndicator.set_text(str(self.pa.ny()))
		self.ZmaxIndicator.set_text(str(self.pa.nz()))

		self.XScaler.set_range(0.0, self.pa.nx())
		self.XScaler.set_value(self.currentX)
		self.YScaler.set_range(0.0, self.pa.ny())
		self.YScaler.set_value(self.currentY)
		self.ZScaler.set_range(0.0, self.pa.nz())
		self.ZScaler.set_value(self.currentZ)

		self.XText.set_text(str(self.currentX))
		self.YText.set_text(str(self.currentY))
		self.ZText.set_text(str(self.currentZ))

		self.DrawPicture(self.window)
		
	def Open(self, widget):
		# A SIMPLE FILE CHOOSE DIALOG
		dialog = gtk.FileChooserDialog("Please select your PA File...", self.window, gtk.FILE_CHOOSER_ACTION_OPEN, None, None)
		dialog.add_button("Cancel", gtk.RESPONSE_CANCEL)
		dialog.add_button("Select", gtk.RESPONSE_OK)
		if self.OpenFileName != os.getcwd():
			dialog.set_current_folder(os.path.split(self.OpenFileName)[0])
		else:
			dialog.set_current_folder(os.getcwd())
		result = dialog.run()
		if result == gtk.RESPONSE_OK:
			self.OpenFileName = dialog.get_filename()
			dialog.destroy()
		else:
			dialog.destroy()
			return 0		
		
		# SO EVERYTHING GOOD, LETS OPEN THE FILE
		self.pa = PA.PA(file = self.OpenFileName)

		self.currentX = self.pa.nx()/2
		self.currentY = self.pa.ny()/2
		self.currentZ = self.pa.nz()/2

		self.XmaxIndicator.set_text(str(self.pa.nx()))
		self.YmaxIndicator.set_text(str(self.pa.ny()))
		self.ZmaxIndicator.set_text(str(self.pa.nz()))

		self.XScaler.set_range(0.0, self.pa.nx())
		self.XScaler.set_value(self.currentX)
		self.YScaler.set_range(0.0, self.pa.ny())
		self.YScaler.set_value(self.currentY)
		self.ZScaler.set_range(0.0, self.pa.nz())
		self.ZScaler.set_value(self.currentZ)

		self.XText.set_text(str(self.currentX))
		self.YText.set_text(str(self.currentY))
		self.ZText.set_text(str(self.currentZ))

		self.DrawPicture(self.window)

	def DrawPicture_thread(self, widget):
		drawFigureThreat().start()

	def DrawPicture(self, widget):
		# Nothing open
		if self.pa == None:
			return 0;

		self.wTree.get_widget("View").set_text(self.OpenFileName)
		# CHECK WHO WANTS TO BE PLOTTED?
		self.TitleOfPlots = []
		if self.wTree.get_widget("CheckRaw").get_active() == True:
			self.TitleOfPlots.append("Raw")
		if self.wTree.get_widget("CheckGeo").get_active() == True:
			self.TitleOfPlots.append("Geometry")
		if self.wTree.get_widget("CheckPot").get_active() == True:
			self.TitleOfPlots.append("Potential")

		# CLEAR FIGURE IS IMPORTANT!
		self.figure.clear()
		self.ax = []
		i = 0
		while i < len(self.TitleOfPlots):
			num = 100 + 10 * len(self.TitleOfPlots) + 1 + i
			self.ax.append(self.figure.add_subplot(num))
			if self.plane == "xy":
				self.ax[i].set_xlabel('Y')
				self.ax[i].set_ylabel('X')
			elif self.plane == "xz":		
				self.ax[i].set_xlabel('Z')
				self.ax[i].set_ylabel('X')
			elif self.plane == "yz":
				self.ax[i].set_xlabel('Z')
				self.ax[i].set_ylabel('Y')
			self.ax[i].set_title(self.TitleOfPlots[i])
			i = i + 1
		data_raw = []
		data_geo = []
		data_pot = []
		if self.plane == "xy":
			i = 0
			while i < self.pa.nx():
				j = 0
				data_raw.append([])
				data_geo.append([])
				data_pot.append([])
				while j < self.pa.ny():
					data_pot[i].append(self.pa.potential(i,j,self.currentZ))
					data_geo[i].append(self.pa.electrode(i,j,self.currentZ))
					data_raw[i].append(self.pa.raw(i,j,self.currentZ))
					j = j + 1
				i = i + 1
		elif self.plane == "xz":
			i = 0
			while i < self.pa.nx():
				j = 0
				data_raw.append([])
				data_geo.append([])
				data_pot.append([])
				while j < self.pa.nz():
					data_pot[i].append(self.pa.potential(i,self.currentY,j))
					data_geo[i].append(self.pa.electrode(i,self.currentY,j))
					data_raw[i].append(self.pa.raw(i,self.currentY,j))
					j = j + 1
				i = i + 1
		elif self.plane == "yz":
			i = 0
			while i < self.pa.ny():
				j = 0
				data_raw.append([])
				data_geo.append([])
				data_pot.append([])
				while j < self.pa.nz():
					data_pot[i].append(self.pa.potential(self.currentX,i,j))				
					data_geo[i].append(self.pa.electrode(self.currentX,i,j))
					data_raw[i].append(self.pa.raw(self.currentX,i,j))
					j = j + 1
				i = i + 1
		i = 0
		while i < len(self.TitleOfPlots):
			if self.TitleOfPlots[i] == "Raw":
				self.ax[i].imshow(data_raw)
				self.figure.colorbar(self.ax[i].imshow(data_raw), ax=self.ax[i], shrink=(1.1 - len(self.TitleOfPlots)/10.0), pad=0.01)
			elif self.TitleOfPlots[i] == "Geometry":
				self.ax[i].imshow(data_geo)
				self.figure.colorbar(self.ax[i].imshow(data_geo), ax=self.ax[i], shrink=(1.1 - len(self.TitleOfPlots)/10.0), pad=0.01)
			elif self.TitleOfPlots[i] == "Potential":
				self.ax[i].imshow(data_pot)
				self.figure.colorbar(self.ax[i].imshow(data_pot), ax=self.ax[i], shrink=(1.1 - len(self.TitleOfPlots)/10.0), pad=0.01)

			i = i + 1
		self.canvas.draw()
		
	def ChangePlane(self, widget):
		if self.plane == "xy":
			self.plane = "xz"
			self.PlaneIndicator.set_text("XZ")
		elif self.plane == "xz":
			self.plane = "yz"
			self.PlaneIndicator.set_text("YZ")
		elif self.plane == "yz":
			self.plane = "xy"
			self.PlaneIndicator.set_text("XY")
		self.DrawPicture(self.window)

	def ChangeCurrent(self, widget):
		if self.lock == 1:
			return 0;
		self.lock = 1
		if widget.get_name() == "ScaleX":
			self.currentX = int(self.XScaler.get_value())
		elif widget.get_name() == "ScaleY":
			self.currentY = int(self.YScaler.get_value())
		elif widget.get_name() == "ScaleZ":
			self.currentZ = int(self.ZScaler.get_value())
#		elif widget.get_name() == "CheckRaw" or widget.get_name() == "CheckGeo" or widget.get_name() == "CheckPot":
			
		else:
			self.currentX = int(float(self.XText.get_text()))
			self.currentY = int(float(self.YText.get_text()))
			self.currentZ = int(float(self.ZText.get_text()))
		self.XText.set_text(str(self.currentX))
		self.YText.set_text(str(self.currentY))
		self.ZText.set_text(str(self.currentZ))
		self.XScaler.set_value(self.currentX)
		self.YScaler.set_value(self.currentY)
		self.ZScaler.set_value(self.currentZ)
		self.DrawPicture(self.window)
		self.lock = 0
	
	def SaveFigure(self, widget):
		# A SIMPLE FILE CHOOSE DIALOG
		dialog = gtk.FileChooserDialog("Please select your PA File...", self.window, gtk.FILE_CHOOSER_ACTION_SAVE, None, None)
		dialog.add_button("Cancel", gtk.RESPONSE_CANCEL)
		dialog.add_button("Select", gtk.RESPONSE_OK)
		result = dialog.run()
		if result == gtk.RESPONSE_OK:
			FileName = dialog.get_filename()
			dialog.destroy()
		else:
			dialog.destroy()
			return 0		
		self.figure.savefig((FileName + ".pdf"))

if __name__ == "__main__":
	hwg = appGui()
	if(len(sys.argv) > 1):
		hwg.DirectOpen(sys.argv[1])
	gtk.main()

