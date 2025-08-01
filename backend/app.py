# -- Part One: Basic Libraries and Settings
#1 Libraries
import psycopg2 #To interact with the PostgreSQL database
from psycopg2 import extras #For DictCursor
from flask import Flask, jsonify #Import the Flask framework to create the API and the jsonify function
from flask_cors import CORS #To enable Cross-Origin Resource Sharing (CORS) for the API
import os #To access environment variables
from dotenv import load_dotenv #To load variables from the .env file

#Create an application object 
app = Flask(__name__)
CORS(app) #Enable CORS on the Flask app to allow requests from the frontend

#2 Database Connection Settings
#Load credentials securely from environment variables defined in the .env file
load_dotenv() 
DB_CONFIG = {
    'host': os.getenv('DB_HOST'),
    'port': os.getenv('DB_PORT'),
    'user': os.getenv('DB_USER'),
    'password': os.getenv('DB_PASSWORD'),
    'database': os.getenv('DB_DATABASE')
}

# -- Part Two: Database Connection Function
#Helper function to get a database connection 
def get_db_connection():
    try:
        #Use psycopg2.connect to connect to PostgreSQL
        conn = psycopg2.connect(**DB_CONFIG)
        return conn
    except psycopg2.Error as err: # 'err' is the variable holding the exception object, which contains details about the error
        print(f"Error connecting to the database: {err}")
        return None

# -- Part three: Creating the Table and Entering Data (For the First Time) 
#1 Function to create the services table (if it doesn't exist)
def create_services_table():
    conn = get_db_connection()
    if conn is None:
        print("Failed to connect to the database, cannot create table.")
        return

    cursor = conn.cursor()
    try:
        #Create the table with the desired schema
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS services (
                id SERIAL PRIMARY KEY,
                service_name VARCHAR(255) NOT NULL,
                title VARCHAR(255) NOT NULL,
                provider_name VARCHAR(255) NOT NULL,
                location VARCHAR(255),
                service_description TEXT,
                rating DECIMAL(3, 2),
                price_per_hour DECIMAL(10, 2) NOT NULL
            );
        """)
        conn.commit() #Save changes to the database
        print("Services table created successfully")
    except psycopg2.Error as err: # 'err' is the variable holding the exception object, which contains details about the error
        print(f"Error creating table: {err}")
    finally: #The 'finally' block ensures that resources are always released
        if cursor:
            cursor.close()
        if conn:
            conn.close()

#2 Function to insert sample data (only if table is empty) 
# This function inserts the 10 detailed service data only if the table is currently empty
def insert_sample_services():
    conn = get_db_connection()
    if conn is None:
        print("Failed to connect to the database, cannot insert data.")
        return

    cursor = conn.cursor()
    try:
        #Check if the table already has data
        cursor.execute("SELECT COUNT(*) FROM services;")
        count_result = cursor.fetchone()  
        count = count_result[0] if count_result else 0

        if count == 0: #Only insert if the table is empty
            #Data for ten services
            services_data = [
                ('Plumbing', 'Expert Plumbing Fix', 'Ahmed Musa', 'Khartoum', 'Professional plumbing services including leak repairs, pipe installations, and drainage solutions.', 4.75, 5000.00),
                ('Electrical Repair', 'Home Wiring Expert', 'Sami Idris', 'Omdurman', 'Certified electrician available for home wiring, socket installation, and light fixture repair.', 4.60, 4500.00),
                ('AC Maintenance', 'CoolAir Maintenance', 'CoolFix Sudan', 'Bahri', 'Air conditioner maintenance, gas refilling, and filter cleaning for homes and offices.', 4.85, 6000.00),
                ('Painting', 'Quick Home Painting', 'Alwan Plus', 'Port Sudan', 'Interior and exterior painting services with color consultation and quick turnaround.', 4.40, 4000.00),
                ('Carpentry', 'Precision Carpentry', 'Abdelrahman Koko', 'Khartoum', 'Custom furniture, door and window repairs, and built-in shelving installation.', 4.70, 5500.00),
                ('Cleaning Services', 'Shiny Homes', 'Shiny Homes Sudan', 'Nyala', 'Reliable cleaning team offering deep home and office cleaning services.', 4.90, 3000.00),
                ('Satellite Installation', 'SudSat Services', 'Yasir Elamin', 'Kassala', 'Satellite dish installation, setup, and troubleshooting for all TV packages.', 4.50, 3500.00),
                ('Handyman', 'Fix-It-All Guy', 'FixIt Hub', 'Khartoum North', 'One-stop handyman services: mounting, repairs, furniture assembly and more.', 4.65, 4000.00),
                ('Tile Work', 'Pro Tiling Services', 'Hiba Farouk', 'El Obeid', 'Bathroom and kitchen tile work, sealing, and water-resistant finishing.', 4.55, 4800.00),
                ('Generator Repair', 'PowerFix Repairs', 'PowerFix Engineering', 'Al-Fashir', 'Repairs and maintenance for residential and commercial generators.', 4.35, 5500.00)
            ]

            #Use executemany to insert multiple rows
            cursor.executemany("""
                INSERT INTO services (service_name, title, provider_name, location, service_description, rating, price_per_hour) VALUES
                (%s, %s, %s, %s, %s, %s, %s);
            """, services_data)
            conn.commit() #Save changes to the database
            print("Inserted 10 services successfully.")
        else:
            print("The services data already exists in the table, and the insertion process was skipped.")
    except psycopg2.Error as err: # 'err' is the variable holding the exception object, which contains details about the error.
        print(f"Error inserting data: {err}")
    finally: #The 'finally' block ensures that resources are always released
        if cursor:
            cursor.close()
        if conn:
            conn.close()

# -- Part Four: JSON Data Initialization Function
#Helper function to format database row into desired JSON structure 
def format_service_for_json(row):
    #Return a regular dictionary
    return {
        "serviceName": row.get("service_name"),
        "Title": row.get("title"),
        "providerName": row.get("provider_name"),
        "location": row.get("location"),
        "serdescription": row.get("service_description"),
        "rating": row.get("rating", 0.0), #Default to 0.0 if rating is None
        "pricePerHour": row.get("price_per_hour", 0.0) #Default to 0.0 if price is None
    }

# -- Part Five: API endpoint to retrieve all services 
@app.route('/api/services', methods=['GET'])
def get_all_services():
    conn = get_db_connection()
    if conn is None:
        print("Failed to connect to the database, cannot retrieve services.")
        return jsonify({"error": "Failed to connect to the database"}), 500

    cursor = conn.cursor(cursor_factory=psycopg2.extras.DictCursor)
    services_list = []
    try:
        #Select all columns from the services table
        cursor.execute("SELECT id, service_name, title, provider_name, location, service_description, rating, price_per_hour FROM services")
        raw_services = cursor.fetchall()
        # Format each raw service row into the desired JSON structure
        services_list = [format_service_for_json(dict(row)) for row in raw_services]
    except psycopg2.Error as err: # 'err' is the variable holding the exception object, which contains details about the error
        print(f"Error querying the database: {err}")
        return jsonify({"error": "Failed to retrieve services"}), 500
    finally: #The 'finally' block ensures that resources are always released
        if cursor:
            cursor.close()
        if conn:
            conn.close()

    #Use jsonify to convert the list to standard JSON format
    return jsonify(services_list)

 
# -- Part Six: Running the Application
if __name__ == '__main__':
    # These functions are called once to set up the database and insert initial data.
    #create_services_table() 
    #insert_sample_services() 
 app.run(port=5001)
