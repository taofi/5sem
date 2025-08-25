CREATE VIEW segment_info AS
SELECT 
    segment_name,
    segment_type,
    COUNT(segment_name) AS segment_count,
    SUM(extents) AS extent_count,
    SUM(blocks) AS block_count,
    SUM(bytes) / 1024 AS size_kb
FROM 
    dba_segments
GROUP BY 
    segment_name,
    segment_type;
    
SELECT * FROM segment_info;

