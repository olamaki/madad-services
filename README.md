Madad Services Backend APIThis repository contains the backend API for the "Madad Services" application, built using Flask and PostgreSQL. It provides a single endpoint to retrieve a list of services, with a setup designed for easy deployment and secure configuration.Key FeaturesRESTful API: A simple GET endpoint to fetch a list of services in JSON format.PostgreSQL Database: Utilizes a PostgreSQL database hosted on the Render platform.Initial Database Seeding: The code includes functions to automatically create the services table and insert initial sample data upon the first run. These functions are commented out for subsequent runs to prevent re-creation.Secure Credentials: Database connection details are loaded from a .env file, ensuring sensitive information is kept out of the public repository.CORS Enabled: The API is configured with CORS to allow requests from frontend applications hosted on different domains.PrerequisitesMake sure you have the following installed on your system:Python 3.xpip (Python package manager)InstallationClone the repository:# Note: Replace 'olamaki' with your actual GitHub username.
git clone https://github.com/olamaki/madad-services.git
cd madad-services
Create and activate a virtual environment:python -m venv venv
# On Windows:
venv\Scripts\activate
# On macOS and Linux:
source venv/bin/activate
Install the required dependencies:The project uses the following libraries: Flask, psycopg2-binary, python-dotenv, and Flask-Cors.pip install Flask psycopg2-binary python-dotenv Flask-Cors
ConfigurationCreate a .env file:In the root directory of your project, create a file named .env and add your PostgreSQL database connection credentials from Render.Example .env file:DB_HOST=your_db_host
DB_PORT=5432
DB_USER=your_db_user
DB_PASSWORD=your_db_password
DB_DATABASE=your_db_database
Important: Replace the placeholder values with your actual credentials. This file should be in your .gitignore to prevent it from being uploaded to GitHub.How to Run the ApplicationFirst-time setup:To set up the database, uncomment the create_services_table() and insert_sample_services() lines at the bottom of the app.py file.The if __name__ == '__main__': block should look like this:if __name__ == '__main__':
    create_services_table()
    insert_sample_services()
    app.run(port=5001)
Run the server for the first time to initialize the database:python app.py
You will see console output confirming the table creation and data insertion. After this, you can stop the server (Ctrl+C).Subsequent runs:Comment out the two setup lines again to prevent the functions from running every time you start the app.The if __name__ == '__main__': block should now look like this:if __name__ == '__main__':
    #create_services_table()
    #insert_sample_services()
    app.run(port=5001)
Run the server normally:python app.py
API EndpointYou can access all the services via the following endpoint:Endpoint: GET /api/servicesExample URL: http://127.0.0.1:5001/api/servicesExample Response:[
  {
    "serviceName": "Plumbing",
    "Title": "Expert Plumbing Fix",
    "providerName": "Ahmed Musa",
    "location": "Khartoum",
    "serdescription": "Professional plumbing services including leak repairs, pipe installations, and drainage solutions.",
    "rating": 4.75,
    "pricePerHour": 5000.0
  },
  {
  "serviceName": "Electrical Repair",
  "Title": "Home Wiring Expert",
  "providerName": "Sami Idris",
  "location": "Omdurman",
  "serdescription": "Certified electrician available for home wiring, socket installation, and light fixture repair.",
  "rating": 4.6,
  "pricePerHour": 4500.0
  }
]
Source CodeThis project is built using:Flask: A lightweight Python web framework.psycopg2: A library for connecting to a PostgreSQL database.python-dotenv: To handle environment variables for secure configuration.Flask-Cors: An extension to enable CORS support.
