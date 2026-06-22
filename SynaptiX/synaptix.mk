DEVICE_SOURCE = ${SYNAPTIXDIR}/board/board.c \
				${SYNAPTIXDIR}/board/mcp320x.c \
				${SYNAPTIXDIR}/board/button.c \
				${SYNAPTIXDIR}/utils/cqueue.c \
				${SYNAPTIXDIR}/services/logger.c \
				${SYNAPTIXDIR}/services/shell/cli_shell.c \
				${SYNAPTIXDIR}/services/lora/lora.c \
				${SYNAPTIXDIR}/apps/app.c \
				${SYNAPTIXDIR}/apps/input.c \
				${SYNAPTIXDIR}/apps/output.c \
				${SYNAPTIXDIR}/apps/user_app.c \
				${SYNAPTIXDIR}/apps/rs4852RF.c \
				${SYNAPTIXDIR}/apps/shell_app.c \
				${SYNAPTIXDIR}/apps/cli_shell_command.c \
				${SYNAPTIXDIR}/apps/app_settings.c \
				${SYNAPTIXDIR}/apps/rf_app.c \
				${SYNAPTIXDIR}/apps/indicator.c \
				${SYNAPTIXDIR}/apps/analogs.c \
				${SYNAPTIXDIR}/services/zigbee/zigbee.c

MODBUS_SOURCE =	${SYNAPTIXDIR}/services/Modbus/modbus/ascii/mbascii.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/rtu/mbcrc.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/rtu/mbrtu.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/functions/mbfunccoils.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/functions/mbfuncdiag.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/functions/mbfuncdisc.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/functions/mbfuncholding.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/functions/mbfuncinput.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/functions/mbfuncother.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/functions/mbutils.c \
				${SYNAPTIXDIR}/services/Modbus/modbus/mb.c \
				${SYNAPTIXDIR}/services/mbport/portevent.c \
				${SYNAPTIXDIR}/services/mbport/portserial.c \
				${SYNAPTIXDIR}/services/mbport/porttimer.c \
				${SYNAPTIXDIR}/apps/user_mb_app.c \

DEVICE_INCLUDE = -I{SYNAPTIXDIR}/apps \
				 -I{SYNAPTIXDIR}/board \
				 -I{SYNAPTIXDIR}/services \
				 -I{SYNAPTIXDIR}/utils

		