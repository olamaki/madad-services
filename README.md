# Madad Services Backend API

This repository contains the backend for the "Madad Services" application, a simple Flask API that interacts with a PostgreSQL database hosted on Render.

The API provides a single endpoint to fetch a list of available services in JSON format, designed for a frontend application to display them to users.

## Key Features

* **RESTful API:** A single GET endpoint (`/api/services`) to retrieve all available services.
* **PostgreSQL Database:** Uses a PostgreSQL database hosted on the Render platform.
* **Initial Seeding:** The application can automatically create the `services` table and insert initial sample data on its first run.
* **Secure Configuration:** Database credentials are loaded securely from environment variables (`.env`).
* **CORS Support:** Cross-Origin Resource Sharing (CORS) is enabled to allow frontend applications on different domains to access the API.

## Prerequisites

To run this project, you need:
* Python 3.x
* pip (Python package manager)
* A PostgreSQL database (e.g., on Render)

## Installation

1.  **Clone the repository:**
    ```
    git clone git@github.com:olamaki/madad-services.git
    cd madad-services
    ```

2.  **Create and activate a virtual environment:**
    ```sh
    python -m venv venv
    ```

    # For Windows:
    ```sh
    .\venv\Scripts\activate
    ```
    # For macOS and Linux:
    ```sh
    source venv/bin/activate
    ```

3.  **Install the required dependencies:**
    ```sh
    pip install Flask psycopg2-binary python-dotenv Flask-Cors
    ```

4.  **Set up environment variables:**
    Create a file named `.env` in the project's root directory and add your database connection details. This file should be in your `.gitignore`.

    Example `.env` file:
    ```env
    DB_HOST=your_db_host
    DB_PORT=5432
    DB_USER=your_db_user
    DB_PASSWORD=your_db_password
    DB_DATABASE=your_db_database
    ```

## How to Run the Application

1.  **First-time setup:**
    * To create the database table and insert the initial data, uncomment the `create_services_table()` and `insert_sample_services()` lines at the bottom of your `app.py` file.
    * The `if __name__ == '__main__':` block should look like this:
        ```python
        if __name__ == '__main__':
            create_services_table()
            insert_sample_services()
            app.run(port=5001)
        ```
    * Run the server once to initialize the database:
        ```sh
        python app.py
        ```
    * You will see messages confirming the table creation and data insertion. After this, stop the server (`Ctrl+C`).

2.  **Subsequent runs:**
    * Comment out the two setup lines again to prevent them from running on every startup.
    * The `if __name__ == '__main__':` block should now look like this:
        ```python
        if __name__ == '__main__':
            #create_services_table()
            #insert_sample_services()
            app.run(port=5001)
        ```
    * Run the server normally:
        ```sh
        python app.py
        ```

## API Endpoint

You can access all the services via this endpoint:

* **Endpoint:** `GET /api/services`
* **Example URL:** `http://127.0.0.1:5001/api/services`

## Source Code

This project is built using:
* **Flask:** A Python web framework.
* **psycopg2:** A library to connect to PostgreSQL.
* **python-dotenv:** For managing environment variables.
* **Flask-Cors:** To enable Cross-Origin Resource Sharing (CORS).
