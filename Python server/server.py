from twisted.internet import protocol, reactor
import re
import MySQLdb
import time

db_name = 'testdb'
db_host = 'localhost'
user_name = '*'
user_password = '*'

db = MySQLdb.connect(host=db_host, user=user_name, passwd=user_password, db=db_name, charset='ascii')
cursor = db.cursor()

class Twist(protocol.Protocol):
	def connectionMade(self):
		print 'connection success!'
	def dataReceived(self, data):
		try:
			x = time.time()
			message = ''
			#print data
			door = data.split('::')[0]
			card = data.split('::')[1]
			cursor.execute("SELECT count(*) from card_door where card_id = " + str(card) + " and door_id = " + door)
			db.commit()
			data = cursor.fetchall()
			for rec in data:
				acc = int(rec[0])
			if acc == 0:
				message = "Deny"
				print door + '::'+ card + message
				self.transport.write(message)
			else:
				cursor.execute("select weekday(now())")
				db.commit()
				data = cursor.fetchall()
				for rec in data:
					dow = int(rec[0])
				day = ''
				if dow == 0: day = 'mon'
				elif dow == 1: day = 'tue'
				elif dow == 2: day = 'wed'
				elif dow == 3: day = 'thu'
				elif dow == 4: day = 'fri'
				elif dow == 5: day = 'sat'
				elif dow == 6: day = 'sun'
				cursor.execute("select " + day + " from card_door where card_id = " + str(card) + " and door_id = " + door)
				db.commit()
				data = cursor.fetchall()
				for rec in data:
					atime = str(rec[0])
				if atime == '0':
					message = "Deny" 
					print door + '::'+ card + " " + message
                                	self.transport.write(message)
				elif atime == '1':
					message = "Allow"
                                        print door + '::'+ card + " " + message
                                        self.transport.write(message)
				else:
					result = re.findall(r'\[([\d:-]+)\]', atime)
					x_t = time.localtime(x)
					for res in result:
						#print res
						t1 = res.split('-')[0]
						t2 = res.split('-')[1]
						#print str(x_t[3]) + " " + str(x_t[4])
						if int(x_t[3]) >= int(t1.split(':')[0]) and int(x_t[4]) >= int(t1.split(':')[1]) and int(x_t[3]) == int(t2.split(':')[0]) and int(x_t[4]) <= int(t2.split(':')[1]):
							message = "Allow"
						elif int(x_t[3]) >= int(t1.split(':')[0]) and int(x_t[4]) >= int(t1.split(':')[1]) and int(x_t[3]) < int(t2.split(':')[0]):
							message = "Allow"
						else:
							message = "Deny"
					print door + '::'+ card + " " + message
					self.transport.write(message)

			cursor.execute("select name from users where card_id = " + str(card))
			db.commit()
			data = cursor.fetchall()
			for rec in data:
				name = str(rec[0])

			cursor.execute('select name from doors where id = ' + door)
			db.commit()
			data = cursor.fetchall()
			for rec in data:
				door_name = str(rec[0])

			cursor.execute('select now()')
			db.commit()
			data = cursor.fetchall()
			for rec in data:
				time2 = str(rec[0])

			cursor.execute("insert into history(name, room, status, time) values(\'"+ name +"\', \'" + door_name +"\', \'" + message +"\', \'"+ time2 +"\')")
			db.commit()

		except:
			m = -1
			#print 'data error'
			#self.transport.write('-1')
	def connectionLost(self, reason):
		print 'Connection lost!'

factory = protocol.Factory()
factory.protocol = Twist
print 'ready'
reactor.listenTCP(5555, factory)
reactor.run()
