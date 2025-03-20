import sys
import random
from PySide6 import QtCore, QtWidgets, QtGui


class VirtualKeyboard(QtWidgets.QWidget):
	def __init__(self):
		super().__init__()
		self.setStyleSheet("background-color: #f0f0f0;")

		layout = QtWidgets.QGridLayout(self)
		layout.setAlignment(QtCore.Qt.AlignCenter)
		layout.setHorizontalSpacing(5)
		layout.setVerticalSpacing(5)

		buttons_row0 = [
			('ESC', 0, 0, 1, 2), ('F1', 0, 2), ('F2', 0, 3), ('F3', 0, 4),
			('F4', 0, 5), ('F5', 0, 6), ('F6', 0, 7), ('F7', 0, 8),
			('F8', 0, 9), ('F9', 0, 10), ('F10', 0, 11), ('F11', 0, 12),
			('F12', 0, 13), ('F13', 0, 14)
		]
		for params in buttons_row0:
			self.add_button(*params)

		buttons_row1 = [
			('~', 1, 0), ('1', 1, 1), ('2', 1, 2), ('3', 1, 3),
			('4', 1, 4), ('5', 1, 5), ('6', 1, 6), ('7', 1, 7),
			('8', 1, 8), ('9', 1, 9), ('0', 1, 10), ('-', 1, 11),
			('+', 1, 12), ('BACKSPACE', 1, 13, 1, 2)
		]
		for params in buttons_row1:
			self.add_button(*params)

		buttons_row2 = [
			('TAB', 2, 0, 1, 2),
			('Q', 2, 2), ('W', 2, 3), ('E', 2, 4), ('R', 2, 5),
			('T', 2, 6), ('Y', 2, 7), ('U', 2, 8), ('I', 2, 9),
			('O', 2, 10), ('P', 2, 11), ('{', 2, 12), ('}', 2, 13),
			('ENTER', 2, 14, 2, 1)
		]
		for params in buttons_row2:
			self.add_button(*params)

		buttons_row3 = [
			('CAPS', 3, 0, 1, 2),
			('A', 3, 2), ('S', 3, 3), ('D', 3, 4), ('F', 3, 5),
			('G', 3, 6), ('H', 3, 7), ('J', 3, 8), ('K', 3, 9),
			('L', 3, 10), (';', 3, 11), ('"', 3, 12), ('\\', 3, 13)
		]
		for params in buttons_row3:
			self.add_button(*params)

		buttons_row4 = [
			('SHIFT', 4, 0, 1, 2),
			('|', 4, 2), ('Z', 4, 3), ('X', 4, 4), ('C', 4, 5),
			('V', 4, 6), ('B', 4, 7), ('N', 4, 8), ('M', 4, 9),
			('<', 4, 10), ('>', 4, 11), ('/', 4, 12), ('SHIFT', 4, 13, 1, 2)
		]
		for params in buttons_row4:
			self.add_button(*params)

		buttons_row5 = [
			('CTRL', 5, 0, 1, 2),
			('Win', 5, 2), ('ALT', 5, 3), ('SPACE', 5, 4, 1, 6), ('ALT', 5, 10),
			('FN', 5, 11, 1, 2), ('CTRL', 5, 13, 1, 2),
		]
		for params in buttons_row5:
			self.add_button(*params)

	def add_button(self, text, row, col, row_span=1, col_span=1):
		btn = QtWidgets.QPushButton(text)

		h_spacing = self.layout().horizontalSpacing()
		v_spacing = self.layout().verticalSpacing()

		base_width = 50
		base_height = 40

		width = base_width * col_span + h_spacing * (col_span - 1)
		height = base_height * row_span + v_spacing * (row_span - 1)

		btn.setFixedSize(int(width), int(height))

		btn.setStyleSheet("""
			QPushButton {
				border: 1px solid #999;
				border-radius: 5px;
				background-color: white;
				font-size: 12px;
			}
			QPushButton:pressed {
				background-color: #ddd;
			}
			""")

		self.layout().addWidget(btn, row, col, row_span, col_span)
		return btn


class VirtualMouse(QtWidgets.QWidget):
	def __init__(self):
		super().__init__()
		self.setStyleSheet("background: transparent;")
		self.setFixedSize(190, 340)

		# Основные кнопки
		self.left_btn = self._create_button("", QtCore.QRect(30, 65, 60, 100), 16)
		self.right_btn = self._create_button("", QtCore.QRect(100, 65, 60, 100), 16)

		# Колесо прокрутки
		self.middle_btn = self._create_button("", QtCore.QRect(80, 180, 30, 50), 10, color="#555")

		# Кнопки под колесом (DPI или макросы)
		self.extra1_btn = self._create_button("", QtCore.QRect(85, 240, 20, 20), 5, color="#444")
		self.extra2_btn = self._create_button("", QtCore.QRect(85, 265, 20, 20), 5, color="#444")

		# Боковые кнопки (слева)
		self.side1_btn = self._create_button("", QtCore.QRect(0, 220, 25, 30), 8, color="#444")
		self.side2_btn = self._create_button("", QtCore.QRect(0, 255, 25, 30), 8, color="#444")

	def _create_button(self, text, geometry, radius, color="#333"):
		btn = QtWidgets.QPushButton(text, self)
		btn.setGeometry(geometry)
		btn.setStyleSheet(f"""
				QPushButton {{
						background: {color};
						border: none;
						border-radius: {radius}px;
				}}
				QPushButton:pressed {{
						background: #666;
				}}
		""")
		return btn

	def sizeHint(self):
		return QtCore.QSize(190, 340)

	def paintEvent(self, event):
		painter = QtGui.QPainter(self)
		painter.setRenderHint(QtGui.QPainter.Antialiasing)

		path = QtGui.QPainterPath()
		path.addRoundedRect(0, 55, 190, 285, 40, 60)
		painter.fillPath(path, QtGui.QColor("#222"))

		painter.setPen(QtGui.QPen(QtGui.QColor("#333"), 2))
		painter.drawLine(95, 65, 95, 170)


class QueueWidget(QtWidgets.QWidget):
	def __init__(self, parent=None):
		super().__init__(parent)
		self.layout = QtWidgets.QVBoxLayout(self)
		self.layout.setContentsMargins(5, 5, 5, 5)
		self.layout.setSpacing(5)
		self.elements = []
		self.element_height = 30
		self.setFixedWidth(150)
		self.setMaximumHeight(250)
		self.setStyleSheet("""
			QWidget {
				background-color: #f0f0f0;
				border: 1px solid #999;
				border-radius: 5px;
			}
		""")

	def set_elements(self, elements_list):
		self.elements = elements_list
		self.update_display()

	def update_display(self):
		while self.layout.count():
			child = self.layout.takeAt(0)
			if child.widget():
				child.widget().deleteLater()

		available_height = self.maximumHeight()
		spacing = self.layout.spacing()
		element_total = self.element_height + spacing
		max_possible = (available_height + spacing) // element_total

		if len(self.elements) > max_possible:
			display_elements = self.elements[:max_possible - 1]
			display_elements.append("...")
		else:
			display_elements = self.elements[:max_possible]

		for text in display_elements:
			label = QtWidgets.QLabel(text)
			label.setStyleSheet("""
				QLabel {
					background-color: white;
					border: 1px solid #999;
					border-radius: 3px;
					padding: 2px;
				}
			""")
			label.setFixedHeight(self.element_height)
			self.layout.addWidget(label)

		num_elements = len(display_elements)
		needed_height = num_elements * (self.element_height + spacing) - spacing
		self.setFixedHeight(min(needed_height, self.maximumHeight()))


class MainWindow(QtWidgets.QWidget):
	def __init__(self):
		super().__init__()
		self.setWindowTitle("Virtual Input")
		self.resize(1280, 720)

		self.keyboard = VirtualKeyboard()
		self.mouse = VirtualMouse()
		self.timer_label = QtWidgets.QLabel("0 seconds")
		timer_font = self.timer_label.font()
		timer_font.setPointSize(25)
		timer_font.setBold(True)
		self.timer_label.setFont(timer_font)
		self.inputs_queue = QueueWidget()
		self.inputs_queue.set_elements([
			"Action 1 [Time: 1]", "Action 2 [Time: 2]", "Action 3 [Time: 3]", "Action 4 [Time: 4]",
			"Action 5 [Time: 5]", "Action 6 [Time: 6]", "Action 7 [Time: 7]", "Action 8 [Time: 8]"
		])

		self.keyboard.setParent(self)
		self.mouse.setParent(self)
		self.timer_label.setParent(self)
		self.inputs_queue.setParent(self)

		self.timer = QtCore.QTimer()
		self.timer.timeout.connect(self.update_time)
		self.start_time = QtCore.QDateTime.currentDateTime()
		self.timer.start(1000)

		self.update_geometries()

	def update_time(self):
		elapsed_time = self.start_time.secsTo(QtCore.QDateTime.currentDateTime())
		self.timer_label.setText(f"{elapsed_time} seconds")

	def update_geometries(self):
		keyboard_width = self.keyboard.sizeHint().width()
		keyboard_height = self.keyboard.sizeHint().height()
		self.keyboard.setGeometry(
			(self.width() - keyboard_width) // 2,  # X координата
			self.height() - keyboard_height,  # Y координата
			keyboard_width,  # Ширина
			keyboard_height  # Высота
		)

		mouse_width = self.mouse.sizeHint().width()
		mouse_height = self.mouse.sizeHint().height()
		self.mouse.setGeometry(
			self.width() - mouse_width,
			self.height() - mouse_height,
			mouse_width,
			mouse_height
		)

		timer_width = self.timer_label.sizeHint().width()
		timer_height = self.timer_label.sizeHint().height()
		self.timer_label.setGeometry(
			(self.width() - timer_width) // 2,
			0,
			timer_width,
			timer_height
		)
		queue_width = self.inputs_queue.sizeHint().width()
		queue_height = self.inputs_queue.sizeHint().height()
		self.inputs_queue.setGeometry(
			self.width() - queue_width - 35,
			10,
			queue_width,
			queue_height
		)

	def resizeEvent(self, event):
		self.update_geometries()
		super().resizeEvent(event)


if __name__ == "__main__":
	app = QtWidgets.QApplication([])

	window = MainWindow()
	window.show()

	sys.exit(app.exec())
