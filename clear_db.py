import psycopg2

# --- Database Connection Settings ---
# استخدمي نفس بيانات الاتصال اللي في ملف app.py بتاعكِ
DB_CONFIG = {
    'host': 'dpg-d24fupggjchc73a9urr0-a.frankfurt-postgres.render.com',
    'port': '5432',
    'user': 'madad_user',
    'password': 'iNTWzvPNACpkDLLPzorNTGhl7hXFljdY', # تأكدي إن دي كلمة المرور الصحيحة
    'database': 'madad_services_db'
}

def drop_services_table():
    conn = None
    cursor = None
    try:
        conn = psycopg2.connect(**DB_CONFIG)
        cursor = conn.cursor()

        # مسح الجدول بالكامل لو موجود
        cursor.execute("DROP TABLE IF EXISTS services CASCADE;")
        conn.commit()
        print("Old 'services' table dropped (if it existed).")
        print("Database is now clean and ready for initial setup from app.py.")

    except psycopg2.Error as err:
        print(f"Error in dropping table: {err}")
    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()

if __name__ == '__main__':
    drop_services_table()
