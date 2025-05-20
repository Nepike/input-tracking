import sys
import inputs_module as inputs
from PySide6 import QtCore, QtWidgets, QtGui

keys = {
	QtCore.Qt.Key_Escape: 'ESC',
	QtCore.Qt.Key_F1: 'F1',
	QtCore.Qt.Key_F2: 'F2',
	QtCore.Qt.Key_F3: 'F3',
	QtCore.Qt.Key_F4: 'F4',
	QtCore.Qt.Key_F5: 'F5',
	QtCore.Qt.Key_F6: 'F6',
	QtCore.Qt.Key_F7: 'F7',
	QtCore.Qt.Key_F8: 'F8',
	QtCore.Qt.Key_F9: 'F9',
	QtCore.Qt.Key_F10: 'F10',
	QtCore.Qt.Key_F11: 'F11',
	QtCore.Qt.Key_F12: 'F12',
	QtCore.Qt.Key_F13: 'F13',
	QtCore.Qt.Key_AsciiTilde: '~',
	QtCore.Qt.Key_1: '1',
	QtCore.Qt.Key_2: '2',
	QtCore.Qt.Key_3: '3',
	QtCore.Qt.Key_4: '4',
	QtCore.Qt.Key_5: '5',
	QtCore.Qt.Key_6: '6',
	QtCore.Qt.Key_7: '7',
	QtCore.Qt.Key_8: '8',
	QtCore.Qt.Key_9: '9',
	QtCore.Qt.Key_0: '0',
	QtCore.Qt.Key_Minus: '-',
	QtCore.Qt.Key_Plus: '+',
	QtCore.Qt.Key_Backspace: 'BACKSPACE',
	QtCore.Qt.Key_Tab: 'TAB',
	QtCore.Qt.Key_Q: 'Q',
	QtCore.Qt.Key_W: 'W',
	QtCore.Qt.Key_E: 'E',
	QtCore.Qt.Key_R: 'R',
	QtCore.Qt.Key_T: 'T',
	QtCore.Qt.Key_Y: 'Y',
	QtCore.Qt.Key_U: 'U',
	QtCore.Qt.Key_I: 'I',
	QtCore.Qt.Key_O: 'O',
	QtCore.Qt.Key_P: 'P',
	QtCore.Qt.Key_BraceLeft: '{',
	QtCore.Qt.Key_BraceRight: '}',
	QtCore.Qt.Key_Return: 'ENTER',
	QtCore.Qt.Key_CapsLock: 'CAPS',
	QtCore.Qt.Key_A: 'A',
	QtCore.Qt.Key_S: 'S',
	QtCore.Qt.Key_D: 'D',
	QtCore.Qt.Key_F: 'F',
	QtCore.Qt.Key_G: 'G',
	QtCore.Qt.Key_H: 'H',
	QtCore.Qt.Key_J: 'J',
	QtCore.Qt.Key_K: 'K',
	QtCore.Qt.Key_L: 'L',
	QtCore.Qt.Key_Semicolon: ';',
	QtCore.Qt.Key_QuoteDbl: '"',
	QtCore.Qt.Key_Backslash: '\\',
	QtCore.Qt.Key_Shift: 'SHIFT',
	QtCore.Qt.Key_Bar: '|',
	QtCore.Qt.Key_Z: 'Z',
	QtCore.Qt.Key_X: 'X',
	QtCore.Qt.Key_C: 'C',
	QtCore.Qt.Key_V: 'V',
	QtCore.Qt.Key_B: 'B',
	QtCore.Qt.Key_N: 'N',
	QtCore.Qt.Key_M: 'M',
	QtCore.Qt.Key_Less: '<',
	QtCore.Qt.Key_Greater: '>',
	QtCore.Qt.Key_Slash: '/',
	QtCore.Qt.Key_Control: 'CTRL',
	QtCore.Qt.Key_Meta: 'Win',
	QtCore.Qt.Key_Alt: 'ALT',
	QtCore.Qt.Key_Space: 'SPACE',
}

mouse_buttons = {
	QtCore.Qt.LeftButton: 'left_btn',
	QtCore.Qt.RightButton: 'right_btn',
	QtCore.Qt.MiddleButton: 'middle_btn',
	QtCore.Qt.BackButton: 'extra1_btn',
	QtCore.Qt.ForwardButton: 'extra2_btn',
}

numerical_mouse = {
	'left_btn': 1,
	'right_btn' : 2,
	'middle_btn': 3,
	'extra1_btn' : 4,
	'extra2_btn':5
}


class VirtualKeyboard(QtWidgets.QWidget):
	def __init__(self):
		super().__init__()
		self.buttons = {}
		self.setStyleSheet("background-color: #f0f0f0;")
		layout = QtWidgets.QGridLayout(self)
		layout.setAlignment(QtCore.Qt.AlignCenter)
		layout.setHorizontalSpacing(5)
		layout.setVerticalSpacing(5)

		# все строки кнопок (пример для первых двух, дальше по аналогии)
		rows = [
			[
				('ESC', 0, 0, 1, 2), ('F1', 0, 2), ('F2', 0, 3), ('F3', 0, 4),
				('F4', 0, 5), ('F5', 0, 6), ('F6', 0, 7), ('F7', 0, 8),
				('F8', 0, 9), ('F9', 0, 10), ('F10', 0, 11), ('F11', 0, 12),
				('F12', 0, 13), ('F13', 0, 14)
			],
			[
				('~', 1, 0), ('1', 1, 1), ('2', 1, 2), ('3', 1, 3),
				('4', 1, 4), ('5', 1, 5), ('6', 1, 6), ('7', 1, 7),
				('8', 1, 8), ('9', 1, 9), ('0', 1, 10), ('-', 1, 11),
				('+', 1, 12), ('BACKSPACE', 1, 13, 1, 2)
			],
			[
				('TAB', 2, 0, 1, 2),
				('Q', 2, 2), ('W', 2, 3), ('E', 2, 4), ('R', 2, 5),
				('T', 2, 6), ('Y', 2, 7), ('U', 2, 8), ('I', 2, 9),
				('O', 2, 10), ('P', 2, 11), ('{', 2, 12), ('}', 2, 13),
				('ENTER', 2, 14, 2, 1)
			],
			[
				('CAPS', 3, 0, 1, 2),
				('A', 3, 2), ('S', 3, 3), ('D', 3, 4), ('F', 3, 5),
				('G', 3, 6), ('H', 3, 7), ('J', 3, 8), ('K', 3, 9),
				('L', 3, 10), (';', 3, 11), ('"', 3, 12), ('\\', 3, 13)
			],
			[
				('SHIFT', 4, 0, 1, 2),
				('|', 4, 2), ('Z', 4, 3), ('X', 4, 4), ('C', 4, 5),
				('V', 4, 6), ('B', 4, 7), ('N', 4, 8), ('M', 4, 9),
				('<', 4, 10), ('>', 4, 11), ('/', 4, 12), ('SHIFT', 4, 13, 1, 2)
			],
			[
				('CTRL', 5, 0, 1, 2),
				('Win', 5, 2), ('ALT', 5, 3), ('SPACE', 5, 4, 1, 6), ('ALT', 5, 10),
				('FN', 5, 11, 1, 2), ('CTRL', 5, 13, 1, 2)
			]
		]

		for row in rows:
			for params in row:
				self.add_button(*params)

	def add_button(self, text, row, col, row_span=1, col_span=1):
		btn = QtWidgets.QPushButton(text)
		base_w, base_h = 50, 40
		h_sp, v_sp = self.layout().horizontalSpacing(), self.layout().verticalSpacing()
		btn.setFixedSize(base_w * col_span + h_sp * (col_span - 1),
		                 base_h * row_span + v_sp * (row_span - 1))
		# общий стиль
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
		self.buttons[text] = btn
		return btn

	def highlight_key(self, key_text):
		if key_text not in self.buttons:
			return
		self.buttons[key_text].setStyleSheet("""
			QPushButton {
				border: 1px solid #999;
				border-radius: 5px;
				background-color: #ddd;
				font-size: 12px;
			}
			QPushButton:pressed {
				background-color: #ddd;
			}
		""")

	def lowlight_key(self, key_text):
		if key_text not in self.buttons:
			return
		self.buttons[key_text].setStyleSheet("""
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


class VirtualMouse(QtWidgets.QWidget):
	def __init__(self):
		super().__init__()
		self.buttons = {}
		self.setStyleSheet("background: transparent;")
		self.setFixedSize(190, 340)
		self.left_btn = self._create_button("left_btn", QtCore.QRect(30, 65, 60, 100))
		self.right_btn = self._create_button("right_btn", QtCore.QRect(100, 65, 60, 100))
		self.middle_btn = self._create_button("middle_btn", QtCore.QRect(80, 180, 30, 50))
		self.side1_btn = self._create_button("extra2_btn", QtCore.QRect(0, 220, 25, 30))
		self.side2_btn = self._create_button("extra1_btn", QtCore.QRect(0, 255, 25, 30))

	def _create_button(self, name, rect):
		btn = QtWidgets.QPushButton("", self)
		btn.setGeometry(rect)
		btn.setStyleSheet(f"""
			QPushButton {{
				background: #555;
				border: none;
				border-radius: {rect.width() // 2}px;
			}}
			QPushButton.pressed {{
				background: #333;
			}}
			QPushButton[highlighted="true"] {{
				background: #333;
			}}
		""")
		self.buttons[name] = btn
		return btn

	def sizeHint(self):
		return QtCore.QSize(190, 340)

	def highlight_mouse(self, name: str):
		btn = self.buttons.get(name)
		if not btn:
			return
		btn.setProperty("highlighted", True)
		btn.style().unpolish(btn)
		btn.style().polish(btn)

	def lowlight_mouse(self, name: str):
		btn = self.buttons.get(name)
		if not btn:
			return
		btn.setProperty("highlighted", False)
		btn.style().unpolish(btn)
		btn.style().polish(btn)

	def paintEvent(self, event):
		painter = QtGui.QPainter(self)
		painter.setRenderHint(QtGui.QPainter.Antialiasing)
		path = QtGui.QPainterPath()
		path.addRoundedRect(0, 55, 190, 285, 40, 60)
		painter.fillPath(path, QtGui.QColor("#222"))
		painter.setPen(QtGui.QPen(QtGui.QColor("#333"), 2))
		painter.drawLine(95, 65, 95, 170)


class QueueWidget(QtWidgets.QWidget):
    def __init__(self, max_width=450, max_height=800, parent=None):
        super().__init__(parent)
        self.setMinimumWidth(max_width)
        self.setMaximumWidth(max_width)
        self.setMaximumHeight(max_height)

        self.text_edit = QtWidgets.QTextEdit(self)
        self.text_edit.setReadOnly(True)
        self.text_edit.setLineWrapMode(QtWidgets.QTextEdit.NoWrap)
        self.text_edit.setStyleSheet(
            """
            QTextEdit {
                background-color: #f0f0f0;
                border: 1px solid #999;
                border-radius: 5px;
                padding: 5px;
                font-size: 14px;
            }
            """
        )

        layout = QtWidgets.QVBoxLayout(self)
        layout.setContentsMargins(5, 5, 5, 5)
        layout.addWidget(self.text_edit)

        self.elements = []

    def set_elements(self, elements_list):
        self.elements = list(elements_list)
        self.text_edit.setPlainText("\n".join(self.elements))

    def add_element(self, text: str):
        if not text:
            return
        self.elements.append(text)
        if self.text_edit.toPlainText():
            self.text_edit.append(text)
        else:
            self.text_edit.setPlainText(text)

    def clear(self):
        self.elements.clear()
        self.text_edit.clear()


class DraggableTextRect(QtWidgets.QFrame):
	def __init__(self, parent=None, x=50, y=50, w=200, h=100, element_id=1):
		super().__init__(parent)
		self.element_id = element_id
		self.setFrameShape(QtWidgets.QFrame.Box)
		self.setLineWidth(2)
		self.setStyleSheet("background-color: white; border: 2px solid #333;")
		self.setGeometry(x, y, w, h)
		self.setMouseTracking(True)
		self._drag_active = False

		header_height = 25
		self.header = QtWidgets.QWidget(self)
		self.header.setGeometry(0, 0, w, header_height)
		hdr_layout = QtWidgets.QHBoxLayout(self.header)
		hdr_layout.setContentsMargins(5, 0, 5, 0)
		hdr_layout.setSpacing(5)

		title = QtWidgets.QLabel(f"ControlElement {self.element_id}", self.header)
		title.setStyleSheet("font-weight: bold;")
		hdr_layout.addWidget(title)
		hdr_layout.addStretch()

		close_btn = QtWidgets.QPushButton("×", self.header)
		close_btn.setFixedSize(20, 20)
		close_btn.setStyleSheet(
			"QPushButton { border: none; font-weight: bold; }"
			"QPushButton:hover { color: red; }"
		)
		close_btn.clicked.connect(self.deleteLater)
		hdr_layout.addWidget(close_btn)

		self.text_edit = QtWidgets.QTextEdit(self)
		self.text_edit.setGeometry(
			5,
			header_height + 5,
			w - 10,
			h - header_height - 10
		)
		self.text_edit.setStyleSheet("border: 1px solid #999;")
		self.text_edit.installEventFilter(parent)

	def mousePressEvent(self, event: QtGui.QMouseEvent):
		if event.button() == QtCore.Qt.LeftButton:
			self._drag_active = True
			self._drag_offset = event.pos()
		super().mousePressEvent(event)

	def mouseMoveEvent(self, event: QtGui.QMouseEvent):
		if self._drag_active:
			new_pos = self.mapToParent(event.pos() - self._drag_offset)
			self.move(new_pos)
		super().mouseMoveEvent(event)

	def mouseReleaseEvent(self, event: QtGui.QMouseEvent):
		if event.button() == QtCore.Qt.LeftButton:
			self._drag_active = False
		super().mouseReleaseEvent(event)


class MainWindow(QtWidgets.QWidget):
	def __init__(self):
		super().__init__()

		self.recording = False
		self.manager = inputs.InputManager()
		self.ctrls_cnt = 0
		self.pressed = set()

		self.setWindowTitle("Inputs Recorder")
		self.resize(1280, 720)
		# self.setFocusPolicy(QtCore.Qt.StrongFocus)

		self.keyboard = VirtualKeyboard()
		self.keyboard.setParent(self)

		self.mouse = VirtualMouse()
		self.mouse.setParent(self)

		self.inputs_queue = QueueWidget()
		self.inputs_queue.set_elements(["BACKEND RESPONSE WILL BE HERE", "BACKEND RESPONSE WILL BE HERE", "BACKEND RESPONSE WILL BE HERE"])
		self.inputs_queue.setParent(self)

		self.add_rect_btn = QtWidgets.QPushButton("Add Control Element", self)
		self.add_rect_btn.setGeometry(10, 60, 150, 40)
		self.add_rect_btn.clicked.connect(self.create_ctrl)

		self.record_btn = QtWidgets.QPushButton("Record Actions", self)
		self.record_btn.setGeometry(10, 110, 150, 40)
		self.record_btn.clicked.connect(self.toggle_record)

		self.play_btn = QtWidgets.QPushButton("Play Actions", self)
		self.play_btn.setGeometry(10, 160, 150, 40)
		self.play_btn.clicked.connect(self.play_actions)

		self.installEventFilter(self)
		QtCore.QTimer.singleShot(0, self.arrange_widgets)

		self.timer_label = QtWidgets.QLabel("Timer")
		font = self.timer_label.font()
		font.setPointSize(25)
		font.setBold(True)
		self.timer_label.setFont(font)
		self.timer_label.setParent(self)

		self.timer = QtCore.QTimer()
		self.timer.timeout.connect(self.update_time)
		self.start_time = QtCore.QDateTime.currentDateTime()

		self.manager.addControlElement("Background", {inputs.InputType.MOUSE, inputs.InputType.TOUCH, inputs.InputType.KEYBOARD})
		self.manager.addControlElement("QLabel", {inputs.InputType.MOUSE, inputs.InputType.TOUCH, inputs.InputType.KEYBOARD})
		self.manager.addControlElement("MainWindow", {inputs.InputType.MOUSE, inputs.InputType.TOUCH, inputs.InputType.KEYBOARD})
		self.manager.addControlElement("QTextEdit", {inputs.InputType.MOUSE, inputs.InputType.TOUCH, inputs.InputType.KEYBOARD})

		self._mouse_swipe_active = False
		self._swipe_start_pos = None



	def arrange_widgets(self):
		kb_w = self.keyboard.sizeHint().width()
		kb_h = self.keyboard.sizeHint().height()
		self.keyboard.setGeometry(
			(self.width() - kb_w) // 2,
			self.height() - kb_h,
			kb_w, kb_h
		)

		m_w = self.mouse.sizeHint().width()
		m_h = self.mouse.sizeHint().height()
		self.mouse.setGeometry(
			self.width() - m_w - 10,
			self.height() - m_h - 5,
			m_w, m_h
		)

		t_w = self.timer_label.sizeHint().width()
		t_h = self.timer_label.sizeHint().height()
		self.timer_label.setGeometry(20, 10, t_w, t_h)

		q_w = self.inputs_queue.sizeHint().width()
		q_h = self.inputs_queue.sizeHint().height()
		self.inputs_queue.setGeometry(
			self.width() - q_w - 200,
			10,
			q_w, q_h
		)

	def resizeEvent(self, event):
		self.arrange_widgets()
		super().resizeEvent(event)

	def update_time(self):
		ms = self.start_time.msecsTo(QtCore.QDateTime.currentDateTime())
		self.timer_label.setText(f"{ms / 1000:.2f}")

	def toggle_record(self):
		self.recording = not self.recording
		if self.recording:
			self.manager.performActions(100000000000000)
			try:
				self.timer.timeout.disconnect(self._play_step)
			except Exception:
				pass
			self.start_time = QtCore.QDateTime.currentDateTime()
			self.record_btn.setText("Stop Recording")
			self.timer.start(20)
		else:
			self.record_btn.setText("Record Actions")
			self.timer.stop()

	def create_ctrl(self):
		self.ctrls_cnt += 1
		rect = DraggableTextRect(self, x=100, y=100, w=250, h=140, element_id=self.ctrls_cnt)
		rect.show()
		self.manager.addControlElement(f"ControlElement {self.ctrls_cnt}", {inputs.InputType.MOUSE, inputs.InputType.TOUCH, inputs.InputType.KEYBOARD})

	def play_actions(self):
		self.start_time = QtCore.QDateTime.currentDateTime()
		self.inputs_queue.clear()
		self.timer.timeout.connect(self._play_step)
		self.timer.start(20)

	def _play_step(self):
		ms = round(self.start_time.msecsTo(QtCore.QDateTime.currentDateTime()) / 1000, 2)
		responses = self.manager.performActions(ms)
		for response in responses:
			self.inputs_queue.add_element(response)

	def eventFilter(self, watched, event):
		def get_context(obj):
			if not obj:
				return "Background"
			if obj.objectName():
				return obj.objectName()
			if isinstance(obj, DraggableTextRect):
				return f"ControlElement {obj.element_id}"
			return obj.__class__.__name__

		# --- клавиатура ---
		if event.type() in (QtCore.QEvent.KeyPress, QtCore.QEvent.KeyRelease):
			if event.isAutoRepeat():
				return False

			key = keys.get(event.key())
			if not key:
				return False

			focused = QtWidgets.QApplication.focusWidget()
			context = get_context(focused)
			action = (inputs.ActionType.PRESS if event.type() == QtCore.QEvent.KeyPress else inputs.ActionType.RELEASE)

			if action == inputs.ActionType.PRESS:
				self.keyboard.highlight_key(key)
			else:
				self.keyboard.lowlight_key(key)

			if self.recording:
				ms = round(self.start_time.msecsTo(QtCore.QDateTime.currentDateTime()) / 1000, 2)
				self.manager.addKeyboardAction(key, ms, action, context)
			return False

		# --- мышь ---
		if event.type() == QtCore.QEvent.MouseButtonPress:
			btn_name = mouse_buttons.get(event.button())
			if not btn_name:
				return False

			self._mouse_swipe_active = True
			self._swipe_start_pos = event.globalPos()  # глобальные координаты

			target = watched.childAt(event.pos())
			if not target:
				target = QtWidgets.QApplication.widgetAt(event.globalPos())

			context = get_context(target)
			self.mouse.highlight_mouse(btn_name)

			if self.recording:
				ms = round(self.start_time.msecsTo(QtCore.QDateTime.currentDateTime()) / 1000, 2)
				self.manager.addMouseAction(numerical_mouse[btn_name], ms, inputs.ActionType.PRESS, context)
			return False

		elif event.type() == QtCore.QEvent.MouseButtonRelease:
			btn_name = mouse_buttons.get(event.button())
			if not btn_name:
				return False

			self.mouse.lowlight_mouse(btn_name)

			end_pos = event.globalPos()
			if self._mouse_swipe_active and self._swipe_start_pos:
				start = self._swipe_start_pos
				self._mouse_swipe_active = False
				self._swipe_start_pos = None

				if self.recording:
					ms = round(self.start_time.msecsTo(QtCore.QDateTime.currentDateTime()) / 1000, 2)
					self.manager.addTouchAction(ms, [inputs.FingerCoordinates(start.x(), start.y(), end_pos.x(), end_pos.y())],  "MainWindow")

			target = watched.childAt(event.pos())
			if not target:
				target = QtWidgets.QApplication.widgetAt(event.globalPos())
			context = get_context(target)

			if self.recording:
				ms = round(self.start_time.msecsTo(QtCore.QDateTime.currentDateTime()) / 1000, 2)
				self.manager.addMouseAction(numerical_mouse[btn_name], ms, inputs.ActionType.RELEASE, context)
			return False

		return super().eventFilter(watched, event)


if __name__ == "__main__":
	app = QtWidgets.QApplication([])
	window = MainWindow()
	window.show()
	sys.exit(app.exec())
